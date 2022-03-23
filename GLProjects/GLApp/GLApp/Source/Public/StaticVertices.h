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
		glm::vec3(0.2f, 0.5f, 0.2f),//Green,

		glm::vec3(0.2f, 0.2f, 0.5f),//blue

		glm::vec3(0.5f, 0.2f, 0.2f)//red
	};

	
	glm::vec3 SunDir = glm::vec3(-0.5f, -2.0f, 1.f);
	glm::vec3 SunPos = glm::vec3(0.2f, 20.f, -10.f);

};

#endif // !MostSV