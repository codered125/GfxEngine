#pragma once

#include "Source/Public/Rendering/RenderTexture/RenderTextureCubeMapIrradence.h"

//-------------------------------------------------------------------

#include "Source/Public/Math.h"
#include "Source/Public/Meshes/Cube.h"
#include "Source/Public/Meshes/Quad.h"
#include "Source/Public/EngineDebugHelper.h"
#include "Source/Public/Rendering/RenderTarget/SceneRenderTarget.h"
#include "Source/Public/Rendering/RenderTexture/RenderTexture.h"
#include "Source/Public/Shader.h"

//-------------------------------------------------------------------

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <iostream>
#include <SOIL2/src/SOIL2/SOIL2.h>
#include <stb_image.h>

//-------------------------------------------------------------------

RenderTextureCubeMapIrradence::RenderTextureCubeMapIrradence(GLenum InTargetType, GLenum InInternalFormat, GLenum InFormat, const GLchar* InHDRPath)
{
	Params.TargetType = InTargetType;
	Params.InternalFormat = InInternalFormat;
	Params.Format = InFormat;

	IrrandenceRenderBuffer = new SceneRenderTarget(512, 512, GL_TEXTURE_2D, InInternalFormat, InFormat, 1, false, false, true);
	HDRRenderTexture = new RenderTexture(GL_TEXTURE_2D, InInternalFormat, InFormat, InHDRPath, GL_FLOAT, nullptr);
	UnConvolutedMap = new RenderTextureCubeMap(GL_TEXTURE_CUBE_MAP, InInternalFormat, InFormat, 512, 512, false);

	auto CaptureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
	glm::mat4 CaptureViews[] =
	{
		MoMath::MoLookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		MoMath::MoLookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		MoMath::MoLookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
		MoMath::MoLookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
		MoMath::MoLookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		MoMath::MoLookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
	};

	auto EquirectangleToCubemapShader = &Shader("Shaders/IrradenceMapCapture/EquirectangularToCubemap.vs", "Shaders/IrradenceMapCapture/EquirectangularToCubemap.frag");
	EquirectangleToCubemapShader->use();
	EquirectangleToCubemapShader->SetSampler("EquirectangularMap", &HDRRenderTexture->GetID(), GL_TEXTURE_2D);

	glViewport(0, 0, 512, 512);
	glBindFramebuffer(GL_FRAMEBUFFER, IrrandenceRenderBuffer->GetID());
	for (unsigned int i = 0; i < 6; ++i)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, UnConvolutedMap->GetID(), 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Cube Box;
		Box.ThisShader = EquirectangleToCubemapShader;
		Box.Draw(glm::mat4(), CaptureProjection, CaptureViews[i]);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	PreFilteredEnvironmentMap = new RenderTextureCubeMap(GL_TEXTURE_CUBE_MAP, GL_RGB16F, GL_RGB, 128, 128, true);
	auto PrefilterShader = &Shader("Shaders/IrradenceMapCapture/EquirectangularToCubemap.vs", "Shaders/IrradenceMapCapture/PreFilterEnvironmentMap.frag");
	PrefilterShader->use();
	PrefilterShader->SetSampler("UnConvolutedMap", &UnConvolutedMap->GetID(), GL_TEXTURE_CUBE_MAP);

	glBindFramebuffer(GL_FRAMEBUFFER, IrrandenceRenderBuffer->GetID());
	GLuint MaxMipMapLevels = 5;

	for (GLuint MipIndex = 0; MipIndex < MaxMipMapLevels; ++MipIndex)
	{
		// reisze framebuffer according to mip-level size.
		GLuint MipWidth = static_cast<GLuint>(128 * std::pow(0.5, MipIndex));
		GLuint MipHeight = static_cast<GLuint>(128 * std::pow(0.5, MipIndex));
		IrrandenceRenderBuffer->ResizeRenderTarget(MipWidth, MipHeight, GL_DEPTH24_STENCIL8);
		glViewport(0, 0, MipWidth, MipHeight);


		float roughness = (float)MipIndex / (float)(MaxMipMapLevels - 1);
		PrefilterShader->setFloat("MipMapRoughness", roughness);
		for (unsigned int i = 0; i < 6; ++i)
		{
			PrefilterShader->setMat4("view", CaptureViews[i]);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, PreFilteredEnvironmentMap->GetID(), MipIndex);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			Cube Box;
			Box.ThisShader = PrefilterShader;
			Box.Draw(glm::mat4(), CaptureProjection, CaptureViews[i]);
		}
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);



	BRDFLookUpTexture = new RenderTexture(512, 512, GL_TEXTURE_2D, GL_RG16F, GL_RG, false, GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE, NULL, GL_FLOAT, nullptr);
	auto BRDFLookUpShader = &Shader("Shaders/IrradenceMapCapture/BRDFLookUpTexture.vs", "Shaders/IrradenceMapCapture/BRDFLookUpTexture.frag");

	glBindFramebuffer(GL_FRAMEBUFFER, IrrandenceRenderBuffer->GetID());
	IrrandenceRenderBuffer->ResizeRenderTarget(512, 512, GL_DEPTH24_STENCIL8);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, BRDFLookUpTexture->GetID(), 0);
	glViewport(0, 0, 512, 512);
	BRDFLookUpShader->use();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Quad* Plane = new Quad(BRDFLookUpShader);
	Plane->Draw(BRDFLookUpShader);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);





	IrradenceDiffuse = new RenderTextureCubeMap();
	glGenTextures(1, &IrradenceDiffuse->GetID());
	glBindTexture(InTargetType, IrradenceDiffuse->GetID());
	for (GLuint i = 0; i < 6; ++i)
	{
		// note that we store each face with 16 bit floating point values
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, InInternalFormat, 32, 32, 0, InFormat, GL_FLOAT, nullptr);
	}
	glTexParameteri(InTargetType, GL_TEXTURE_WRAP_S, Params.WrapS);
	glTexParameteri(InTargetType, GL_TEXTURE_WRAP_T, Params.WrapT);
	glTexParameteri(InTargetType, GL_TEXTURE_WRAP_R, Params.WrapR);
	glTexParameteri(InTargetType, GL_TEXTURE_MIN_FILTER, Params.MinFilter);
	glTexParameteri(InTargetType, GL_TEXTURE_MAG_FILTER, Params.MagFilter);

	IrrandenceRenderBuffer->ResizeRenderTarget(32, 32, GL_DEPTH24_STENCIL8);

	auto ConvolutionShader = &Shader("Shaders/IrradenceMapCapture/EquirectangularToCubemap.vs", "Shaders/IrradenceMapCapture/IrradenceConvolution.frag");
	ConvolutionShader->use();
	ConvolutionShader->SetSampler("UnConvolutedMap", &UnConvolutedMap->GetID(), GL_TEXTURE_CUBE_MAP);

	glViewport(0, 0, 32, 32);
	glBindFramebuffer(GL_FRAMEBUFFER, IrrandenceRenderBuffer->GetID());
	for (unsigned int i = 0; i < 6; ++i)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, IrradenceDiffuse->GetID(), 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Cube Box;
		Box.ThisShader = ConvolutionShader;
		Box.Draw(glm::mat4(), CaptureProjection, CaptureViews[i]);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


}

//-------------------------------------------------------------------

RenderTextureCubeMap* RenderTextureCubeMapIrradence::GetUnConvolutedRenderTexture()
{
	return UnConvolutedMap;
}

//-------------------------------------------------------------------

RenderTextureCubeMap* RenderTextureCubeMapIrradence::GetPrefilteredEnvironmentMap()
{
	return PreFilteredEnvironmentMap;
}

//-------------------------------------------------------------------

RenderTextureCubeMap* RenderTextureCubeMapIrradence::GetIrradenceDiffuse()
{
	return IrradenceDiffuse;
}

//-------------------------------------------------------------------

RenderTexture* RenderTextureCubeMapIrradence::GetBRDFLookUpTexture()
{
	return BRDFLookUpTexture;
}

//-------------------------------------------------------------------

RenderTexture * RenderTextureCubeMapIrradence::GetHDRRenderTexture()
{
	return HDRRenderTexture;
}

//-------------------------------------------------------------------

RenderTextureCubeMapIrradence::~RenderTextureCubeMapIrradence()
{
	if (IrrandenceRenderBuffer)
	{
		delete IrrandenceRenderBuffer;
		IrrandenceRenderBuffer = nullptr;
	}

	if (HDRRenderTexture)
	{
		delete HDRRenderTexture;
		HDRRenderTexture = nullptr;
	}

	if (UnConvolutedMap)
	{
		delete UnConvolutedMap;
		UnConvolutedMap = nullptr;
	}
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
