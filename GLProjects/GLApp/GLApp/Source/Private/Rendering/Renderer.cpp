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
	MainCamera = new Camera(glm::vec3(0.0f, 10.0f, 0.0f));
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

void Renderer::InitialiseLightingDataForShader(Shader * lightShader)
{
	const float pointIntes = 5; const float directIntes = 15;
	// Directional light
	Directional0 = new DirectionalLight(lightShader, "dirLight");
	Directional0->direction = TheMostStaticVertices::SunDir;// Camera::GetFront(LightingCamera);
	Directional0->ambient = glm::vec3(1);
	Directional0->diffuse = glm::vec3(1.0f, 1.f, 1.f);
	Directional0->specular = glm::vec3(1);
	Directional0->position = TheMostStaticVertices::DebugSunPos;
	Directional0->intensity = directIntes;
	Directional0->setUpShader();

	// Point light 1
	for (int i = 0; TheMostStaticVertices::pointLightColours->length() > i; i++)
	{
		auto Point = PointLight(lightShader, "pointLights[" + std::to_string(i) + "]");
		Point.diffuse = TheMostStaticVertices::pointLightColours[i];
		Point.position = TheMostStaticVertices::pointLightPositions[i]; // MyLerp(pointLightPositions[1], pointLightPositions[0], SecondCounter);
		Point.ambient = glm::vec3(1.0f);
		Point.specular = glm::vec3(1.0f);
		Point.intensity = pointIntes;
		Point.setUpShader();
	}
}

//-------------------------------------------------------------------

void Renderer::DrawLights(Camera * Perspective, Shader* LightShader)
{
	LightShader->use();
	Cube LightCube;
	LightCube.Colour = glm::vec3(50, 50, 50);

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
		LightCube.Colour = TheMostStaticVertices::pointLightColours[i];
		LightCube.Draw(Model, FOV, View);
	}
}

//-------------------------------------------------------------------

std::string Renderer::GetGameTimeAsString()
{
	return std::to_string(glfwGetTime());
}


//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------

