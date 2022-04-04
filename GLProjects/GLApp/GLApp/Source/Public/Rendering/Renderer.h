#pragma once
#include <gl/glew.h>
#include <glm.hpp>
#include <string>

//-------------------------------------------------------------------

class Shader;
class SkyBox;
class Camera;
class Quad;
class RenderTextureSSAO;
class SceneRenderTarget;
class Shape;
struct Light;
struct PostProcessSettings;

//-------------------------------------------------------------------

enum RenderStage {Depth, Main, PostProcess, GBuffer};

//-------------------------------------------------------------------

class Renderer
{
public:

	Renderer(GLint InScreenWidth, GLint InScreenHeight);
	
	PostProcessSettings* MainPostProcessSetting;
	virtual void RenderLoop(float TimeLapsed) = 0;
	virtual void DrawGizmos(Camera* Perspective);
	
	void SetMainCamera(Camera* InCamera);
	Camera* GetMainCamera();

	virtual ~Renderer();

protected:

	void InitialiseLightingDataForShader(Shader * lightShader);

	void DrawLights(Camera* Perspective, Shader* LightShader);
	std::string GetGameTimeAsString();

	GLint SCREEN_WIDTH;
	GLint SCREEN_HEIGHT;

	SceneRenderTarget* MainRenderBuffer;
	SceneRenderTarget* IntermediateRenderBuffer;
	SceneRenderTarget* DepthRenderBuffer;
	SceneRenderTarget* GBuffer;
	SceneRenderTarget* PostProcessRenderBuffer;

	RenderTextureSSAO* SSAOBuilder;

	GLint AliasingCount = 4;
	GLint NumberofLights = 3;

	Light* Directional0 = nullptr;
	Quad* PostProcessingQuad;
	float GameTimeLapsed;
	float GameTimeDelta;
	
	Camera* MainCamera;
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------