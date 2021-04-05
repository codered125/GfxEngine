#pragma once

#include "Source/Public/Rendering/Renderer.h"

//-------------------------------------------------------------------

class Shape;
class Model;
class SkyBox;
class RenderTextureCubeMap;

//-------------------------------------------------------------------

class ForwardRenderer :	public Renderer
{
public:

	ForwardRenderer(int InScreenWidth, int InScreenHeight);

	virtual void RenderLoop(float TimeLapsed) override;


protected:

	void RenderDemo(RenderStage RenderStage, SkyBox* InSkybox, Camera* Perspective, GLuint* ShadowMap);

private:

	void DrawModel(Shader* ModelShader, Shape* InModel, glm::mat4 model, Camera* Perspective, GLuint* ShadowMap);
	void DrawWater(Shader* ModelShader, Model* InModel, glm::mat4 model, Camera* Perspective, float TimeLapsed);
	virtual void DrawGizmos(Camera* Perspective) override;

	Shader* UnlitShader;
	Shader* PBRshader;
	Shader* SkyboxShader;
	Shader* LampShader;
	Shader* DepthShader;
	Shader* ScreenShader;
	Shader* WaterShader;
	Shader* IrradenceMapShader;

	Model* Sponza;
	Model* GizMo;
	Model* WaterBlock;
	Model* ArrowLight;

	SkyBox* VisualSkybox;
	RenderTextureCubeMap* IrradenceMap;
	RenderTextureCubeMap* EquirectangularMap;

};

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------