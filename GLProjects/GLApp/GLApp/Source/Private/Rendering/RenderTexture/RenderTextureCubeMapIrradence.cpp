#pragma once

#include "Source/Public/Rendering/RenderTexture/RenderTextureCubeMapIrradence.h"

//-------------------------------------------------------------------

#include "Source/Public/Meshes/Cube.h"
#include "Source/Public/EngineDebugHelper.h"
#include "Source/Public/Rendering/RenderTarget/SceneRenderTarget.h"
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
	IrrandenceRenderBuffer = new SceneRenderTarget(512, 512, GL_TEXTURE_2D, InInternalFormat, InFormat, 1, false, false, true);
	HDRRenderTexture = new RenderTextureCubeMap(GL_TEXTURE_2D, InInternalFormat, InFormat, InHDRPath);
	UnConvolutedMap = new RenderTextureCubeMap(GL_TEXTURE_CUBE_MAP, InInternalFormat, InFormat, 512, 512, false);

	auto CaptureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
	glm::mat4 CaptureViews[] =
	{
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
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


	glGenTextures(1, &Id);
	glBindTexture(InTargetType, Id);
	for (GLuint i = 0; i < 6; ++i)
	{
		// note that we store each face with 16 bit floating point values
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, InInternalFormat, 32, 32, 0, InFormat, GL_FLOAT, nullptr);
	}
	glTexParameteri(InTargetType, GL_TEXTURE_WRAP_S, WrapS);
	glTexParameteri(InTargetType, GL_TEXTURE_WRAP_T, WrapT);
	glTexParameteri(InTargetType, GL_TEXTURE_WRAP_R, WrapR);
	glTexParameteri(InTargetType, GL_TEXTURE_MIN_FILTER, MinFilter);
	glTexParameteri(InTargetType, GL_TEXTURE_MAG_FILTER, MagFilter);

	IrrandenceRenderBuffer->ResizeRenderTarget(32, 32);

	auto ConvolutionShader = &Shader("Shaders/IrradenceMapCapture/EquirectangularToCubemap.vs", "Shaders/IrradenceMapCapture/IrradenceConvolution.frag");
	ConvolutionShader->use();
	ConvolutionShader->SetSampler("UnConvolutedMap", &UnConvolutedMap->GetID(), GL_TEXTURE_CUBE_MAP);

	glViewport(0, 0, 32, 32);
	glBindFramebuffer(GL_FRAMEBUFFER, IrrandenceRenderBuffer->GetID());
	for (unsigned int i = 0; i < 6; ++i)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, Id, 0);
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
//-------------------------------------------------------------------
//-------------------------------------------------------------------
