#pragma once
#include <gl/glew.h>
#include <glm.hpp>

//-------------------------------------------------------------------

class Shader;
class SkyBox;
class Camera;
class Quad;
class SceneRenderTarget;
class Shape;
struct PostProcessSettings;
struct Light;

//-------------------------------------------------------------------

enum RenderStage {Depth, Main, PostProcess, GBuffer};


//-------------------------------------------------------------------

class Renderer
{
public:

	Renderer(GLint InScreenWidth, GLint InScreenHeight);

	Camera* MainCamera;
	Camera* LightingCamera;
	PostProcessSettings* MainPostProcessSetting;
	virtual void RenderLoop() = 0;

protected:

	void InitialiseLightingDataForShader(Shader * lightShader);

	void DrawLights(Camera* Perspective, Shader* LightShader);

	GLint SCREEN_WIDTH;
	GLint SCREEN_HEIGHT;

	SceneRenderTarget* MainRenderBuffer;
	SceneRenderTarget* IntermediateRenderBuffer;
	SceneRenderTarget* DepthRenderBuffer;
	SceneRenderTarget* GBuffer;


	GLint AliasingCount = 4;
	GLint NumberofLights = 3;

	Light* Directional0;
	Quad* PostProcessingQuad;
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------