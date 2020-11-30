#pragma once

#include "Source/Public/Rendering/Renderer.h"

//-------------------------------------------------------------------

class Model;
class SkyBox;

//-------------------------------------------------------------------

class ForwardRenderer :	public Renderer
{
public:

	ForwardRenderer(int InScreenWidth, int InScreenHeight);

	virtual void RenderLoop() override;


protected:

	void RenderDemo(RenderStage RenderStage, SkyBox* InSkybox, Camera* Perspective, GLuint* ShadowMap);

private:

	void DrawModel(Shader* ModelShader, Model* InModel, glm::mat4 model, Camera* Perspective, GLuint* ShadowMap);

	Shader* UnlitShader;
	Shader* PBRshader;
	Shader* SkyboxShader;
	Shader* LampShader;
	Shader* DepthShader;
	Shader* ScreenShader;
	Model* Sponza;
	Model* GizMo;
	Model* ArrowLight;
	SkyBox* VisualSkybox;

};

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------