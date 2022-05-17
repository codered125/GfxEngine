#pragma once
#include "Source/Public/Math.h"

#include <math.h>

//-------------------------------------------------------------------

constexpr bool USEGLM = 0;

//-------------------------------------------------------------------

glm::vec3 MoMath::MoLerp(const glm::vec3& a, const glm::vec3& b, const float& t)
{
	return a + ((b - a) * t);
}

//-------------------------------------------------------------------

float MoMath::MoLerpFast(const float a, const float b, const float t)
{
	return a + t * (b - a);
}

//-------------------------------------------------------------------

float MoMath::MoDotProduct(const glm::vec3& a, const glm::vec3& b)
{
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

//-------------------------------------------------------------------

glm::vec3 MoMath::MoCrossProduct(const glm::vec3& a, const glm::vec3& b)
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

//-------------------------------------------------------------------

float MoMath::MoClamp(float& T, float A, float B)
{
	const float Min = fmin(A, B);
	const float Max = fmax(A, B);

	return fmax(Min, fmin(Max, T));
}

//-------------------------------------------------------------------

glm::vec3 MoMath::MoNormalize(const glm::vec3& T)
{
	const float TLen = sqrt((T.x * T.x) + (T.y * T.y) + (T.z * T.z));
	return glm::vec3((T.x / TLen), (T.y / TLen), (T.z / TLen));
}

//-------------------------------------------------------------------

glm::mat4x4 MoMath::MoLookAt(const glm::vec3 Position, const glm::vec3 Target, const glm::vec3& WorldUp)
{
	if (USEGLM)
	{
		return glm::lookAt(Position, Target, WorldUp);
	}

	glm::vec3 Forward = MoNormalize(Target - Position); //ZAxis
	glm::vec3 Right = MoNormalize(MoCrossProduct(Forward, WorldUp)); // XAxis
	glm::vec3 Up = MoCrossProduct(Right, Forward); //YAxis

	glm::mat4x4 ViewMatrix;
	ViewMatrix[0][0] = Right.x;
	ViewMatrix[1][0] = Right.y;
	ViewMatrix[2][0] = Right.z;
	ViewMatrix[0][1] = Up.x;
	ViewMatrix[1][1] = Up.y;
	ViewMatrix[2][1] = Up.z;
	ViewMatrix[0][2] = -Forward.x;
	ViewMatrix[1][2] = -Forward.y;
	ViewMatrix[2][2] = -Forward.z;
	ViewMatrix[3][0] = -MoDotProduct(Right, Position);
	ViewMatrix[3][1] = -MoDotProduct(Up, Position);
	ViewMatrix[3][2] = MoDotProduct(Forward, Position);
	return ViewMatrix;
}

//-------------------------------------------------------------------

glm::mat4x4 MoMath::MoOrthographic(const float& Left, const float& Right, const float& Bottom, const float& Top, const float& Near_Plane, const float& Far_Plane)
{
	if (USEGLM)
	{
		return glm::ortho(Left, Right, Bottom, Top, Near_Plane, Far_Plane);
	}

	glm::mat4x4 Result;

	Result[0][0] = 2.0f / (Right - Left);
	Result[1][1] = 2.0f / (Top - Bottom);
	Result[2][2] = -2.0f / (Far_Plane - Near_Plane);

	Result[3][0] = -(Right + Left) / (Right - Left);
	Result[3][1] = -(Top + Bottom) / (Top - Bottom);
	Result[3][2] = -(Far_Plane + Near_Plane) / (Far_Plane - Near_Plane);
	Result[3][3] = 1.0f;

	return Result;
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------