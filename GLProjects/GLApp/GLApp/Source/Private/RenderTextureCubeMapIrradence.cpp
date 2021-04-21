#pragma once

#include "Source/Public/RenderTextureCubeMapIrradence.h"

//-------------------------------------------------------------------

#include "Source/Public/Meshes/Cube.h"
#include "Source/Public/EngineDebugHelper.h"
#include "Source/Public/SceneRenderTarget.h"
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

	glGenTextures(1, &Id);
	glBindTexture(InTargetType, Id);
	for (GLuint i = 0; i < 6; ++i)
	{
		// note that we store each face with 16 bit floating point values
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, InInternalFormat, 512, 512, 0, InFormat, GL_FLOAT, nullptr);
	}
	glTexParameteri(InTargetType, GL_TEXTURE_WRAP_S, WrapS);
	glTexParameteri(InTargetType, GL_TEXTURE_WRAP_T, WrapT);
	glTexParameteri(InTargetType, GL_TEXTURE_WRAP_R, WrapR);
	glTexParameteri(InTargetType, GL_TEXTURE_MIN_FILTER, MinFilter);
	glTexParameteri(InTargetType, GL_TEXTURE_MAG_FILTER, MagFilter);

	auto CaptureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
	glm::mat4 CaptureViews[] =
	{
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
	};

	auto EquirectangleToCubemapShader = &Shader("Shaders/IrradenceMapCapture/EquirectangularToCubemap.vs", "Shaders/IrradenceMapCapture/EquirectangularToCubemap.frag");
	EquirectangleToCubemapShader->use();
	EquirectangleToCubemapShader->setMat4("projection", CaptureProjection);
	EquirectangleToCubemapShader->SetSampler("EquirectangularMap", &HDRRenderTexture->GetID(), GL_TEXTURE_2D);



	glViewport(0, 0, 512, 512); // don't forget to configure the viewport to the capture dimensions.
	glBindFramebuffer(GL_FRAMEBUFFER, IrrandenceRenderBuffer->GetID());
	for (unsigned int i = 0; i < 6; ++i)
	{
		EquirectangleToCubemapShader->setMat4("view", CaptureViews[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, Id, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Cube Box;
		Box.ThisShader = EquirectangleToCubemapShader;
		glm::mat4 Val;
		Box.Draw(Val, CaptureProjection, CaptureViews[i]);
		//renderCube(); // renders a 1x1 cube
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

  //put the whole capture process in here, might be a little suss but we'll make it work then clean it up
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
