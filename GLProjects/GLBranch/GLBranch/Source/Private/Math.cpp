#pragma once
#include "Source/Public/Math.h"

#include <math.h>

//-------------------------------------------------------------------

glm::vec3 MoMath::MoLerp(glm::vec3& a, glm::vec3& b, float& t)
{
	return a + ((b - a) * t);
}

//-------------------------------------------------------------------

float MoMath::MoDotProduct(glm::vec3& a, glm::vec3& b)
{
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

//-------------------------------------------------------------------

glm::vec3 MoMath::MoCrossProduct(glm::vec3& a, glm::vec3& b)
{
	const float x = (a.y * b.z) - (a.z * b.y);
	const float y = (a.z * b.x) - (a.x * b.z);
	const float z = (a.x * b.y) - (a.y * b.x);
	return glm::vec3(x, y, z);
}

//-------------------------------------------------------------------

float MoMath::MoSaturate(float& T)
{
	return static_cast<float>(fmax(fmin(T, 1), 0));
}

//-------------------------------------------------------------------

float MoMath::MoSign(float T)
{
	return static_cast<float>((T > 0) - (T < 0));
}

float MoMath::MoClamp(float& T, float A, float B)
{
	const float Min = fmin(A, B);
	const float Max = fmax(A, B);

	return fmax(Min, fmin(Max, T));
}

//-------------------------------------------------------------------

glm::vec3 MoMath::MoNormalize(glm::vec3& T)
{
	const float TLen = sqrt((T.x * T.x) + (T.y * T.y) + (T.z * T.z));
	return glm::vec3((T.x / TLen), (T.y / TLen), (T.z / TLen));
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------