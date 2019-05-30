#pragma once
#include <vector>
#define GLEW_STATIC
#include <GL/glew.h>
#include <glm.hpp>
#include<gtc/matrix_transform.hpp>
static class MoMath
{
public:
	static glm::vec3 MoLerp(glm::vec3 a, glm::vec3 b, float t)
	{
		return a + ((b - a) * t);
	}

	static float MoDotProduct(glm::vec3 a, glm::vec3 b)
	{
		return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
	}

	static glm::vec3 MoCrossProduct(glm::vec3 a, glm::vec3 b)
	{
		const float x = (a.y * b.z) - (a.z * b.y);
		const float y = (a.z * b.x) - (a.x * b.z);
		const float z = (a.x * b.y) - (a.y * b.x);
		return glm::vec3(x, y, z);
	}

	static float MoSaturate(float T)
	{
		return fmax(fmin(T, 1), 0);
	}

	static float MoSign(float T)
	{
		return (T > 0) - (T < 0);
	}
};