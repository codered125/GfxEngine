#pragma once

#include "Source/Public/Rendering/Renderer.h"
#include "Source/Public/StaticVertices.h"
#include "Source/Public/Camera.h"
#include "Source/Public/Lights/Light.h"
#include "Source/Public/Lights/DirectionalLight.h"
#include "Source/Public/Lights/PointLight.h"
#include "Source/Public/Shader.h"
#include "Source/Public/Meshes/Cube.h"

#include <GLFW/glfw3.h>


#include <string>

//-------------------------------------------------------------------

Renderer::Renderer(GLint InScreenWidth, GLint InScreenHeight)
{
	SCREEN_WIDTH = InScreenWidth;
	SCREEN_HEIGHT = InScreenHeight;
	SetMainCamera( new Camera(glm::vec3(0.0f, 10.0f, 0.0f)));
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
	MainCamera = InCamera;
}

//-------------------------------------------------------------------

Camera* Renderer::GetMainCamera()
{
	return MainCamera;
}

//-------------------------------------------------------------------

void Renderer::InitialiseLightingDataForShader(Shader * lightShader)
{
	//const float pointIntes = 15; const float directIntes = 25;
	//const float pointIntes = 2.5; const float directIntes = 5;
	const float pointIntes = 1; const float directIntes = 1;
	// Directional light
	auto DirectionLight = new DirectionalLight(lightShader, "dirLight");
	DirectionLight->direction = TheMostStaticVertices::SunDir;// Camera::GetFront(LightingCamera);
	DirectionLight->ambient = glm::vec3(1);
	DirectionLight->diffuse = glm::vec3(1.0f, 1.f, 1.f) * 25.0f;
	DirectionLight->specular = glm::vec3(1);
	DirectionLight->position = TheMostStaticVertices::DebugSunPos;
	DirectionLight->intensity = directIntes;
	DirectionLight->SetupShader();

	auto LightSpaceMatrixMapping = LightSpaceMatrixMappings(DirectionLight->GetLightSpaceProjection(), glm::lookAt(-DirectionLight->direction * 10.0f, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f)), DirectionLight->position);
	DirectionLight->AddLightSpaceViewMatrix(LightSpaceMatrixMapping);
	Directional0 = DirectionLight;

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

void Renderer::DrawLights(Camera * Perspective, Shader* LightShader)
{
	LightShader->use();
	Cube LightCube;
	LightCube.Colour = glm::vec3(1.0f, 1.f, 1.f) * 25.0f;

	glm::mat4 FOV = Camera::GetProjection(Perspective);
	glm::mat4 View = Camera::GetViewMatrix(Perspective);
	glm::mat4 Model = glm::mat4();
	Model = glm::translate(Model, TheMostStaticVertices::DebugSunPos); //pointLightPositions[i]);
	Model = glm::scale(Model, glm::vec3(2.f));
	LightCube.ThisShader = LightShader;
	LightCube.Draw(Model, FOV, View);

	for (GLuint i = 0; i < sizeof(TheMostStaticVertices::pointLightPositions); i++)
	{
		Model = glm::mat4();
		Model = glm::translate(Model, TheMostStaticVertices::pointLightPositions[i]); //pointLightPositions[i]);
		Model = glm::scale(Model, glm::vec3(0.2f));
		LightCube.Colour = TheMostStaticVertices::pointLightColours[i] * 15.5f;
		LightCube.Draw(Model, FOV, View);
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
	delete MainCamera;
	MainCamera = nullptr;

	delete Directional0;
	Directional0 = nullptr;
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------

