#pragma once

#include "Source/Public/Rendering/Renderer.h"

//-------------------------------------------------------------------

class Model;
class Shape;
class SkyBox;
class RenderTextureCubeMap;
class RenderTextureCubeMapIrradence;

//-------------------------------------------------------------------

class ForwardRenderer :	public Renderer
{
public:

	ForwardRenderer(int InScreenWidth, int InScreenHeight);

	virtual void RenderLoop(float TimeLapsed) override;
	virtual ~ForwardRenderer();

protected:

	void RenderDemo(RenderStage RenderStage, SkyBox* InSkybox, Camera* Perspective, GLuint* ShadowMap);

private:

	void DrawModel(Shader* ModelShader, Shape* InModel, glm::mat4 model, Camera* Perspective, GLuint* ShadowMap);
	void DrawWater(Shader* ModelShader, Model* InModel, glm::mat4 model, Camera* Perspective, float TimeLapsed);
	virtual void DrawGizmos(Camera* Perspective) override;

	std::unique_ptr<Shader> UnlitShader = nullptr;
	std::unique_ptr<Shader> PBRShader  = nullptr;
	std::unique_ptr<Shader> SkyboxShader = nullptr;
	std::unique_ptr<Shader> LampShader = nullptr;
	std::unique_ptr<Shader> DepthShader = nullptr;
	std::unique_ptr<Shader> ScreenShader = nullptr;
	std::unique_ptr<Shader> WaterShader = nullptr;

	std::unique_ptr<Model> Sponza = nullptr;
	std::unique_ptr<Model> GizMo = nullptr;
	std::unique_ptr<Model> WaterBlock = nullptr;
	std::unique_ptr<Model> ArrowLight = nullptr;

	std::unique_ptr<SkyBox> VisualSkybox = nullptr;
	std::unique_ptr<RenderTextureCubeMapIrradence> IrradenceCapturer = nullptr;

};

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------