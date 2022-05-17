#pragma once

#include "Source/Public/Rendering/RenderTexture/RenderTextureSSAO.h"

//-------------------------------------------------------------------

#include "Source/Public/Math.h"
#include "Source/Public/Meshes/Quad.h"
#include "Source/Public/EngineDebugHelper.h"
#include "Source/Public/Rendering/RenderTarget/SceneRenderTarget.h"
#include "Source/Public/Rendering/RenderTexture/RenderTexture.h"
#include "Source/Public/Shader.h"

//-------------------------------------------------------------------

#include <gtc/matrix_transform.hpp>
#include <iostream>
#include <random>
#include <SOIL2/src/SOIL2/SOIL2.h>
#include <stb_image.h>

//-------------------------------------------------------------------

RenderTextureSSAO::RenderTextureSSAO(int InScreenWidth, int InScreenHeight)
	: NoiseTexture() 
	, SSAOColourBuffer()
	, SSAOBlurBuffer()
	, SSAOKernal()
	, SSAOShader()
{
	auto RandomFloats = std::uniform_real_distribution<float>(0.0f, 1.0f); //Between 0 - 1
	auto Generator = std::default_random_engine();
	const auto SampleCount = 64;
	SSAOKernal.reserve(SampleCount);
	for (GLint i = 0; i < SampleCount; ++i)
	{
		auto Sample = glm::vec3(RandomFloats(Generator) * 2.0f - 1.0f, RandomFloats(Generator) * 2.0f - 1.0f, RandomFloats(Generator));
		Sample = MoMath::MoNormalize(Sample);
		Sample *= RandomFloats(Generator);

		auto Scale = static_cast<float>(i) / SampleCount;
		Scale = MoMath::MoLerpFast(0.1f, 1.0f, Scale * Scale);
		Sample *= Scale;
		SSAOKernal.push_back(Sample);
	}
	
	for (GLuint i = 0; i < 16; i++)
	{
		glm::vec3 Noise(RandomFloats(Generator) * 2.0f - 1.0f, RandomFloats(Generator) * 2.0f - 1.0f, 0.0f);
		SSAONoise.push_back(Noise);
	}
	
	NoiseTexture = std::make_unique<RenderTexture>(4, 4, 0, GL_TEXTURE_2D, GL_RGBA32F, GL_RGB, false, GL_NEAREST, GL_NEAREST, GL_REPEAT, &SSAONoise[0], GL_FLOAT, nullptr);

	SSAOBlurBuffer = std::make_unique<SceneRenderTarget>(InScreenWidth, InScreenHeight, GL_TEXTURE_2D, GL_RED, GL_RED, 1);
	SSAOBlurBuffer.get()->SetColourAttachmentByIndex(&RenderTexture(InScreenWidth, InScreenHeight, 0, GL_TEXTURE_2D, GL_RED, GL_RED, false, GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE, NULL, GL_FLOAT, nullptr), 0);

	SSAOColourBuffer = std::make_unique<SceneRenderTarget>(InScreenWidth, InScreenHeight, GL_TEXTURE_2D, GL_RED, GL_RED, 1);
	SSAOColourBuffer.get()->SetColourAttachmentByIndex(&RenderTexture(InScreenWidth, InScreenHeight, 0, GL_TEXTURE_2D, GL_RED, GL_RED, false, GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE, NULL, GL_FLOAT, nullptr), 0);

	SSAOShader = std::make_unique<Shader>("Shaders/SSAO/ScreenSpaceAmbientOcclusion.vs", "Shaders/SSAO/ScreenSpaceAmbientOcclusion.frag");
	SSAOBlurShader = std::make_unique<Shader>("Shaders/SSAO/ScreenSpaceAmbientOcclusion.vs", "Shaders/SSAO/ScreenSpaceAmbientOcclusionBlur.frag");
}

//-------------------------------------------------------------------

void RenderTextureSSAO::RenderCommandsColour(RenderTexture* InGBufferPos, RenderTexture* InGBufferNormal, glm::mat4 InProjection, glm::mat4 InView)
{
	glBindFramebuffer(GL_FRAMEBUFFER, SSAOColourBuffer->GetID());
	glClear(GL_COLOR_BUFFER_BIT);
	SSAOShader->use();
	SSAOShader->SetSampler("PositionalTexture", &InGBufferPos->GetID(), GL_TEXTURE_2D);
	SSAOShader->SetSampler("NormalTexture", &InGBufferNormal->GetID(), GL_TEXTURE_2D);
	SSAOShader->SetSampler("NoiseTexture", &NoiseTexture->GetID(), GL_TEXTURE_2D);
	SSAOShader->setMat4("Projection", InProjection);
	SSAOShader->setMat4("View", InView);
	
	const auto TextureSize = SSAOColourBuffer->GetColourAttachmentByIndex(0)->GetWidthAndHeightOfTexture();
	SSAOShader->setVec2("ScreenSize", glm::vec2( std::get<0>(TextureSize), std::get<1>(TextureSize)));
	
	GLint Counter = 0;
	for (auto it = SSAOKernal.begin(); it != SSAOKernal.end(); ++it , ++Counter)
	{
		SSAOShader->setVec3("Samples[" + std::to_string(Counter) + "]", SSAOKernal[Counter]);
	}
	std::unique_ptr<Quad>Plane = std::make_unique<Quad>(SSAOShader.get());
	Plane->DrawTrianglesStrip(SSAOShader.get());
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

//-------------------------------------------------------------------

void RenderTextureSSAO::RenderCommandsBlur(RenderTexture* InSSAO)
{
	glBindFramebuffer(GL_FRAMEBUFFER, SSAOBlurBuffer->GetID());
	glClear(GL_COLOR_BUFFER_BIT);
	SSAOBlurShader->use();
	SSAOBlurShader->SetSampler("SSAOTexture", &InSSAO->GetID(), GL_TEXTURE_2D);
	std::unique_ptr<Quad>Plane = std::make_unique<Quad>(SSAOBlurShader.get());
	Plane->DrawTrianglesStrip(SSAOBlurShader.get());
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
}

//-------------------------------------------------------------------

SceneRenderTarget* RenderTextureSSAO::GetSSAOColourBuffer() 
{
	return SSAOColourBuffer.get();
}

//-------------------------------------------------------------------

SceneRenderTarget* RenderTextureSSAO::GetSSAOBlurBuffer() 
{
	return SSAOBlurBuffer.get();
}

//-------------------------------------------------------------------

RenderTexture* RenderTextureSSAO::GetNoiseTexture()
{
	return NoiseTexture.get();
}

//-------------------------------------------------------------------

RenderTextureSSAO::~RenderTextureSSAO()
{
	if (NoiseTexture)
	{
		NoiseTexture.reset();
	}

	if (SSAOBlurBuffer)
	{
		SSAOBlurBuffer.reset();		
	}
	if (SSAOColourBuffer)
	{
		SSAOColourBuffer.reset();
	}

	if (SSAOShader)
	{
		SSAOShader.reset();
	}

}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------

