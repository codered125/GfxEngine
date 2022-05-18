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
	
	std::unique_ptr<PostProcessSettings> MainPostProcessSetting;
	virtual void RenderLoop(float TimeLapsed) = 0;
	virtual void DrawGizmos(Camera* Perspective);
	
	void SetMainCamera(Camera* InCamera);
	Camera* GetMainCamera();

	virtual ~Renderer();

protected:

	void InitialiseLightingDataForShader(Shader * lightShader);
	virtual void InitialiseLightSpaceMatrices();

	void DrawLights(Camera* Perspective, Shader* LightShader);
	std::string GetGameTimeAsString();

	GLint SCREEN_WIDTH;
	GLint SCREEN_HEIGHT;

	std::unique_ptr<SceneRenderTarget> MainRenderBuffer;
	std::unique_ptr<SceneRenderTarget> IntermediateRenderBuffer;
	std::unique_ptr<SceneRenderTarget> DepthRenderBuffer;
	std::unique_ptr<SceneRenderTarget> GBuffer;
	std::unique_ptr<SceneRenderTarget> PostProcessRenderBuffer;
	std::unique_ptr<RenderTextureSSAO> SSAOBuilder;

	GLint AliasingCount = 4;
	GLint NumberofLights = 3;
	GLuint LightMatricesUBO;

	std::unique_ptr<Light> Directional0;
	std::unique_ptr<Quad> PostProcessingQuad;
	float GameTimeLapsed;
	float GameTimeDelta;
	
	std::unique_ptr<Camera> MainCamera;
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------