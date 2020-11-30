#pragma once
#include <gtc/matrix_transform.hpp>
#include <glm.hpp>
#include <GL\glew.h>
#include "Math.h"

#ifndef MostSV
#define MostSV

namespace TheMostStaticVertices
{
	//SmoothShading
	glm::vec3 pointLightPositions[] =
	{
		glm::vec3(1.0, 1.0, 6),//Green

		glm::vec3(-2.f, 1.f, 0.f),//red

		glm::vec3(1.f, 1.f, -6.f)//blue
	};

	glm::vec3 pointLightColours[] =
	{
		glm::vec3(20.0f, 50.0f, 20.0f),//Green,

		glm::vec3(50.0f, 20.0f, 20.0f),//red

		glm::vec3(20.f, 20.f, 50.0f)//blue
	};

	glm::vec3 SunPos = glm::vec3(-15.02f, 25.0f, -15.02f);
	glm::vec3 SunDir(glm::vec3(-2.0f, -1.0f, 0.0f));
	//glm::vec3 SunDir(-20.0f * MoMath::MoNormalize(glm::vec3(SunPos)));
};

#endif // !MostSV