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

	Shader* UnlitShader = nullptr;
	Shader* PBRshader = nullptr;
	Shader* SkyboxShader = nullptr;
	Shader* LampShader = nullptr;
	Shader* DepthShader = nullptr;
	Shader* ScreenShader = nullptr;
	Shader* GBufferShader = nullptr;
	Model* Sponza = nullptr;
	Model* GizMo = nullptr;
	Model* ArrowLight = nullptr;
	SkyBox* VisualSkybox = nullptr;
	RenderTextureCubeMapIrradence* IrradenceCapturer = nullptr;

};

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------