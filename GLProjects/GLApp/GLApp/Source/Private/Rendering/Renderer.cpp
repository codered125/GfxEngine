#pragma once

#include "Source/Public/Rendering/Renderer.h"
#include "Source/Public/StaticVertices.h"
#include "Source/Public/Camera.h"
#include "Source/Public/Light.h"

#include <string>

//-------------------------------------------------------------------

Renderer::Renderer(GLint InScreenWidth, GLint InScreenHeight)
{
	SCREEN_WIDTH = InScreenWidth;
	SCREEN_HEIGHT = InScreenHeight;
	MainCamera = new Camera(glm::vec3(0.0f, 10.0f, 0.0f));
	LightingCamera = new Camera(TheMostStaticVertices::SunPos, TheMostStaticVertices::SunDir, true, 4096 / 4096, 0.10f, 50.5f);

}

//-------------------------------------------------------------------

void Renderer::RenderLoop()
{
}

//-------------------------------------------------------------------

void Renderer::InitialiseLightingDataForShader(Shader * lightShader)
{
	const float pointIntes = 0.02f; const float directIntes = 0.05f;
	// Directional light
	auto Directional0 = Light::DirectionalLight(lightShader, "dirLight");
	Directional0.direction = TheMostStaticVertices::SunDir;//Camera::getFront(LightingCamera);
	Directional0.ambient = glm::vec3(1);
	Directional0.diffuse = glm::vec3(50, 50, 50);
	Directional0.specular = glm::vec3(1);
	Directional0.position = TheMostStaticVertices::SunPos;
	Directional0.intensity = directIntes;
	Directional0.setUpShader();

	// Point light 1
	for (int i = 0; TheMostStaticVertices::pointLightColours->length() > i; i++)
	{
		auto Point = Light::PointLight(lightShader, "pointLights[" + std::to_string(i) + "]");
		Point.diffuse = TheMostStaticVertices::pointLightColours[i];
		Point.position = TheMostStaticVertices::pointLightPositions[i]; // MyLerp(pointLightPositions[1], pointLightPositions[0], SecondCounter);
		Point.ambient = glm::vec3(1.0f);
		Point.specular = glm::vec3(1.0f);
		Point.intensity = pointIntes;
		Point.setUpShader();
	}
}


//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------

