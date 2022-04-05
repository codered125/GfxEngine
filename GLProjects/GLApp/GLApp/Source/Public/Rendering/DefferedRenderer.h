#pragma once

#include "Source/Public/Rendering/Renderer.h"

//-------------------------------------------------------------------

class Model;
class SkyBox;
class RenderTextureCubeMapIrradence;

//-------------------------------------------------------------------

class DefferedRenderer : public Renderer
{
public:

	DefferedRenderer(int InScreenWidth, int InScreenHeight);

	virtual void RenderLoop(float TimeLapsed) override;

	virtual ~DefferedRenderer();


protected:

	void RenderDemo(RenderStage RenderStage, SkyBox* InSkybox, Camera* Perspective, GLuint* ShadowMap);

private:

	void DrawModel(Shader* ModelShader, Model* InModel, glm::mat4 model, Camera* Perspective, GLuint* ShadowMap);

	std::unique_ptr<Shader> UnlitShader = nullptr;
	std::unique_ptr<Shader> PBRshader = nullptr;
	std::unique_ptr<Shader> SkyboxShader = nullptr;
	std::unique_ptr<Shader> LampShader = nullptr;
	std::unique_ptr<Shader> DepthShader = nullptr;
	std::unique_ptr<Shader> ScreenShader = nullptr;
	std::unique_ptr<Shader> GBufferShader = nullptr;
	std::unique_ptr<Model> Sponza = nullptr;
	std::unique_ptr<Model> GizMo = nullptr;
	std::unique_ptr<Model> ArrowLight = nullptr;
	std::unique_ptr<SkyBox> VisualSkybox = nullptr;
	std::unique_ptr<RenderTextureCubeMapIrradence> IrradenceCapturer = nullptr;

};

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------