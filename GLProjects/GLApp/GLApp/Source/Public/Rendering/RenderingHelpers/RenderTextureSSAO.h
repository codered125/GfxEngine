#pragma once

#include <glm.hpp>
#include <memory>
#include <vector>


//-------------------------------------------------------------------

class Quad;
class RenderTexture;
class SceneRenderTarget;
class Shader;

//-------------------------------------------------------------------

class RenderTextureSSAO
{
public:

	RenderTextureSSAO(int InScreenWidth, int InScreenHeight);
	virtual ~RenderTextureSSAO();
	void RenderCommandsColour(RenderTexture* InGBufferPos, RenderTexture* InGBufferNormal, glm::mat4 InProjection, glm::mat4 InView);

	 SceneRenderTarget* GetSSAOColourBuffer();
	 SceneRenderTarget* GetSSAOBlurBuffer();
	 RenderTexture* GetNoiseTexture();
private:

	std::unique_ptr<RenderTexture> NoiseTexture;
	std::unique_ptr<SceneRenderTarget> SSAOColourBuffer;
	std::unique_ptr<SceneRenderTarget> SSAOBlurBuffer;
	std::unique_ptr<Shader> SSAOShader;

	std::vector<glm::vec3>SSAONoise;
	std::vector<glm::vec3> SSAOKernal;
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------


