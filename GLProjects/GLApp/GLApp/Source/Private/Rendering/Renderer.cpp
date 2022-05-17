#pragma once

#include "Source/Public/Rendering/Renderer.h"
#include "Source/Public/StaticVertices.h"
#include "Source/Public/Camera.h"
#include "Source/Public/Lights/Light.h"
#include "Source/Public/Lights/DirectionalLight.h"
#include "Source/Public/Lights/PointLight.h"
#include "Source/Public/Meshes/Cube.h"
#include "Source/Public/Meshes/Quad.h"
#include "Source/Public/Math.h"
#include "Source/Public/PostProcessing.h"
#include "Source/Public/Rendering/RenderTarget/SceneRenderTarget.h"
#include "Source/Public/Rendering/RenderTexture/RenderTextureSSAO.h"
#include "Source/Public/Rendering/RenderingHelpers/CascadingShadowMapHelper.h"
#include "Source/Public/Shader.h"

#include <GLFW/glfw3.h>

#include <string>

//-------------------------------------------------------------------

Renderer::Renderer(GLint InScreenWidth, GLint InScreenHeight)
{
	SCREEN_WIDTH = InScreenWidth;
	SCREEN_HEIGHT = InScreenHeight; 
	float AspectRatio = (float)InScreenWidth / (float)InScreenHeight;
	SetMainCamera(&Camera(AspectRatio, glm::vec3(0.0f, 10.0f, - 3.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
}

//-------------------------------------------------------------------

void Renderer::RenderLoop(float TimeLapsed)
{
	GameTimeDelta = TimeLapsed - GameTimeLapsed;
	GameTimeLapsed = TimeLapsed;
}

//-------------------------------------------------------------------

void Renderer::DrawGizmos(Camera* Perspective)
{
}

//-------------------------------------------------------------------

void Renderer::SetMainCamera(Camera* InCamera)
{
	MainCamera = std::make_unique<Camera>(*InCamera);
}

//-------------------------------------------------------------------

Camera* Renderer::GetMainCamera()
{
	return MainCamera.get();
}

//-------------------------------------------------------------------

void Renderer::InitialiseLightingDataForShader(Shader* lightShader)
{
	//const float pointIntes = 15; const float directIntes = 25;
	//const float pointIntes = 2.5; const float directIntes = 5;
	const float pointIntes = 1; const float directIntes = 1;
	// Directional light
	if (!Directional0.get())
	{
		Directional0 = std::make_unique<DirectionalLight>(lightShader, "dirLight");
	}

	if (const auto DirectionalLightType = static_cast<DirectionalLight*>(Directional0.get()))
	{
		DirectionalLightType->direction = MoMath::MoNormalize(TheMostStaticVertices::SunDir);
		DirectionalLightType->ambient = glm::vec3(1.0f);
		DirectionalLightType->diffuse = glm::vec3(1.0f) * 25.0f;
		DirectionalLightType->specular = glm::vec3(1.0f);
		DirectionalLightType->position = TheMostStaticVertices::SunPos;
		DirectionalLightType->intensity = directIntes;
		DirectionalLightType->SetupNewShader(lightShader);
	}
	
	// Point light 1
	for (int i = 0; TheMostStaticVertices::pointLightColours->length() > i; i++)
	{
		auto Point = PointLight(lightShader, "pointLights[" + std::to_string(i) + "]");
		Point.diffuse = TheMostStaticVertices::pointLightPositions[i] * 15.5f;
		Point.position = TheMostStaticVertices::pointLightPositions[i]; 
		Point.ambient = glm::vec3(1.0f);
		Point.specular = glm::vec3(1.0f);
		Point.intensity = pointIntes;
		Point.SetupShader();
	}
}

//-------------------------------------------------------------------

void Renderer::InitialiseLightSpaceMatrices()
{
	if (!Directional0.get())
	{
		Directional0 = std::make_unique<DirectionalLight>("dirLight");
	}

	if (const auto DirectionalLightType = static_cast<DirectionalLight*>(Directional0.get()))
	{
		const auto MainCam = GetMainCamera();
		const auto Levels = CascadingShadowMapHelper::GetCascadingLevels(Camera::GetFarPlane(MainCam));
		const auto CascadingCount = Levels.size();
		DirectionalLightType->direction = MoMath::MoNormalize(TheMostStaticVertices::SunDir);
		DirectionalLightType->ReserveMatrixMappings(CascadingCount);
		for (GLuint it = 0; it < CascadingCount + 1; ++it)
		{
			if (it == 0)
			{
				DirectionalLightType->AddLightSpaceViewMatrix(CascadingShadowMapHelper::MakeCascadingLightSpaceMatrix(MainCam, Directional0->direction, Camera::GetNearPlane(MainCam), Levels[it]), it);
			}
			else if (it < CascadingCount)
			{
				DirectionalLightType->AddLightSpaceViewMatrix(CascadingShadowMapHelper::MakeCascadingLightSpaceMatrix(MainCam, Directional0->direction, Levels[it - 1], Levels[it]), it);
			}
			else
			{
				DirectionalLightType->AddLightSpaceViewMatrix(CascadingShadowMapHelper::MakeCascadingLightSpaceMatrix(MainCam, Directional0->direction, Levels[it - 1], Camera::GetFarPlane(MainCam)), it);
			}
		}

	}
}

//-------------------------------------------------------------------

void Renderer::DrawLights(Camera * Perspective, Shader* LightShader)
{
	LightShader->use();
	Cube LightCube;
	LightCube.Colour = glm::vec3(1.0f, 1.f, 1.f) * 25.0f;

	glm::mat4 FOV = Camera::GetProjection(Perspective);
	glm::mat4 View = Camera::GetViewMatrix(Perspective);
	glm::mat4 Model = glm::mat4();
	Model = glm::translate(Model, TheMostStaticVertices::SunPos); //pointLightPositions[i]);
	Model = glm::scale(Model, glm::vec3(2.f));
	LightCube.ThisShader = LightShader;
	LightCube.Draw(Model, FOV, View);

	for (GLuint i = 0; i < sizeof(TheMostStaticVertices::pointLightPositions); i++)
	{
		Model = glm::mat4();
		Model = glm::translate(Model, TheMostStaticVertices::pointLightPositions[i]); //pointLightPositions[i]);
		Model = glm::scale(Model, glm::vec3(0.2f));
		LightCube.Colour = TheMostStaticVertices::pointLightColours[i] * 15.5f;
	//	LightCube.Draw(Model, FOV, View);
	}
}

//-------------------------------------------------------------------

std::string Renderer::GetGameTimeAsString()
{
	return std::to_string(glfwGetTime());
}

//-------------------------------------------------------------------

Renderer::~Renderer()
{
	
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------

