#pragma once
#include <vector>
#define GLEW_STATIC
#include <GL/glew.h>
#include <glm.hpp>
#include<gtc/matrix_transform.hpp>

static::glm::vec3 MoLerp(glm::vec3 a, glm::vec3 b, float t)
{
	return a + ((b - a) * t);
}
