#pragma once
#include <gl/glew.h>
#include <glm.hpp>

//-------------------------------------------------------------------

class Shader;
class SkyBox;
class Camera;
class Quad;
class SceneRenderTarget;
struct PostProcessSettings;

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

	GLint SCREEN_WIDTH;
	GLint SCREEN_HEIGHT;

	SceneRenderTarget* MainRenderBuffer;
	SceneRenderTarget* IntermediateRenderBuffer;
	SceneRenderTarget* DepthRenderBuffer;
	SceneRenderTarget* GBuffer;


	GLint AliasingCount = 4;
	GLint NumberofLights = 3;


	Quad* PostProcessingQuad;
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------