#pragma once

#include "Source/Public/Rendering/RenderingHelpers/CascadingShadowMapHelper.h"

//-------------------------------------------------------------------

#include "Source/Public/Camera.h"
#include "Source/Public/Lights/DirectionalLight.h"
#include "Source/Public/Math.h"
#include "Source/Public/EngineDebugHelper.h"
#include "Source/Public/Shader.h"

//-------------------------------------------------------------------

#include <gtc/matrix_transform.hpp>
#include <iostream>
#include <random>
#include <SOIL2/src/SOIL2/SOIL2.h>
#include <stb_image.h>

//-------------------------------------------------------------------

std::vector<glm::vec4> CascadingShadowMapHelper::GetFrustrumCornersInWorldSpace(const glm::mat4& InProj, const glm::mat4& InView)
{
	const auto InverseMatrix = glm::inverse(InProj * InView);
	auto FrustrumCorners = std::vector<glm::vec4>();
	FrustrumCorners.reserve(8);
	for (GLuint x = 0; x < 2; ++x)
	{
		for (GLuint y = 0; y < 2; ++y)
		{
			for (GLuint z = 0; z < 2; ++z)
			{
				const auto Point = InverseMatrix * glm::vec4(2.0f * x - 1.0f, 2.0f * y - 1.0f, 2.0f * z - 1.0f, 1.0f);
				FrustrumCorners.push_back(Point / Point.w);
			}
		}
	}
	return FrustrumCorners;
}

//-------------------------------------------------------------------

glm::vec3 CascadingShadowMapHelper::GetFrustrumLightViewPosition(const std::vector<glm::vec4>& InCorners)
{
	auto LightViewPosition = glm::vec3(0.0f);
	for (const auto& Vector : InCorners)
	{
		LightViewPosition += glm::vec3(Vector);
	}

	return LightViewPosition /= InCorners.size();
}

//-------------------------------------------------------------------

ProjectionTracker CascadingShadowMapHelper::CreateProjectionTracker()
{
	return ProjectionTracker();
}

//-------------------------------------------------------------------

void CascadingShadowMapHelper::AdjustProjectionTrackerToView(const glm::mat4& InLightView, ProjectionTracker& InTracker, const std::vector<glm::vec4>& InCorners)
{
	for (const auto& Corner : InCorners)
	{
		const auto CornerLightViewSpace = InLightView * Corner;
		InTracker.XMin = std::min(InTracker.XMin, CornerLightViewSpace.x);
		InTracker.XMax = std::max(InTracker.XMax, CornerLightViewSpace.x);
		InTracker.YMin = std::min(InTracker.YMin, CornerLightViewSpace.y);
		InTracker.YMax = std::max(InTracker.YMax, CornerLightViewSpace.y);
		InTracker.ZMin = std::min(InTracker.ZMin, CornerLightViewSpace.z);
		InTracker.ZMax = std::max(InTracker.ZMax, CornerLightViewSpace.z);
	}
}

//-------------------------------------------------------------------

glm::mat4 CascadingShadowMapHelper::GetFrustrumLightProjection(ProjectionTracker& Intracker)
{
	// Tune this parameter according to the scene
	constexpr float zMult = 1.0f;
	if (Intracker.ZMin < 0)
	{
		Intracker.ZMin *= zMult;
	}
	else
	{
		Intracker.ZMin /= zMult;
	}

	if (Intracker.ZMax < 0)
	{
		Intracker.ZMax /= zMult;
	}
	else
	{
		Intracker.ZMax *= zMult;
	}

	return MoMath::MoOrthographic(Intracker.XMin, Intracker.XMax, Intracker.YMin, Intracker.YMax, Intracker.ZMin, Intracker.ZMax); 

}

//-------------------------------------------------------------------

glm::mat4 CascadingShadowMapHelper::GetFrustrumLightView(const glm::vec3& LightViewPosition, const glm::vec3& LightDirection, const glm::vec3& UpDirection)
{
	return MoMath::MoLookAt(LightViewPosition + LightDirection, LightViewPosition, UpDirection);
}

//-------------------------------------------------------------------

LightSpaceMatrixMappings CascadingShadowMapHelper::MakeCascadingLightSpaceMatrix(Camera* Perspective, const glm::vec3& InLightDirection, const float InNearPlane, const float InFarPlane)
{
	auto Corners = CascadingShadowMapHelper::GetFrustrumCornersInWorldSpace(Camera::GetProjection(Perspective, InNearPlane, InFarPlane), Camera::GetViewMatrix(Perspective));
	const glm::vec3 LightViewPosition = CascadingShadowMapHelper::GetFrustrumLightViewPosition(Corners);
	const glm::mat4 LightViewMatrix = CascadingShadowMapHelper::GetFrustrumLightView(LightViewPosition + InLightDirection, LightViewPosition, glm::vec3(0.0f, 1.0f, 0.0f));

	auto Tracker = CascadingShadowMapHelper::CreateProjectionTracker();
	CascadingShadowMapHelper::AdjustProjectionTrackerToView(LightViewMatrix, Tracker, Corners);
	glm::mat4 LightProjection = CascadingShadowMapHelper::GetFrustrumLightProjection(Tracker);

	return LightSpaceMatrixMappings(LightProjection, LightViewMatrix, LightViewPosition);
}

//-------------------------------------------------------------------

std::vector<float> CascadingShadowMapHelper::GetCascadingLevels(float FarPlane)
{
	static std::vector<float> CascadingLevels;
	if (!CascadingLevels.size())
	{
		CascadingLevels = std::vector<float>{ FarPlane / 50.0f, FarPlane / 25.0f, FarPlane / 10.0f, FarPlane / 2.0f };
	}
	return CascadingLevels;
}

//-------------------------------------------------------------------

void CascadingShadowMapHelper::AddCascadingLevelsToShader(Shader* Shader, const float FarPlane)
{
	if (Shader)
	{
		const auto Levels = GetCascadingLevels(FarPlane);
		Shader->setInt("CascadeCount", Levels.size());

		for (GLuint i = 0; i < Levels.size() ; ++i)
		{
			Shader->setFloat("CascadingLevelsArray[" + std::to_string(i) + "]", Levels[i]);
		}
	}
}

//-------------------------------------------------------------------

void CascadingShadowMapHelper::AddLightSpaceMatrixToShader(Shader* Shader, DirectionalLight* Light)
{
	if (Shader)
	{
		for (GLuint i = 0; i < Light->GetNumberOfMatrixMappings(); ++i)
		{
			auto LightMatrix = Light->GetLightSpaceMatrix(i);
			if (LightMatrix.has_value())
			{
				const auto LightSpaceMatrix = LightMatrix->LightSpaceProjection * LightMatrix->LightSpaceViewMatrix;
				Shader->setMat4("LightSpaceMatricesArray[" + std::to_string(i) + "]", LightSpaceMatrix);
			}
		}
	}
}

//-------------------------------------------------------------------

GLuint CascadingShadowMapHelper::CreateMatricesUBO(const GLuint InSize)
{
	GLuint UBO;
	glGenBuffers(1, &UBO);
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4x4) * InSize, nullptr, GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, UBO);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	return UBO;
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------

