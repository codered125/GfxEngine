#pragma once


//-------------------------------------------------------------------

#include <vector>
#include <GL/glew.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

//-------------------------------------------------------------------

class MoMath
{
public:
	static glm::vec3 MoLerp(glm::vec3& a, glm::vec3& b, float& t);
	static float MoLerpFast(float a, float b, float t);
	static float MoDotProduct(glm::vec3& a, glm::vec3& b);
	static glm::vec3 MoCrossProduct(glm::vec3& a, glm::vec3& b);
	static float MoSaturate(float& T);
	static float MoSign(float T);
	static float MoClamp(float& T, float A, float B);
	static glm::vec3 MoNormalize(glm::vec3& T);
	static glm::mat4x4 MoLookAt(glm::vec3& Position, glm::vec3& Target, glm::vec3& WorldUp);
	static glm::mat4x4 MoOrthographic(const float& Left, const float& Right, const float& Bottom, const float& Top, const float& Near_Plane, const float& Far_Plane);
};

//-------------------------------------------------------------------



//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------