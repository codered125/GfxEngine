#pragma once

#include <GL/glew.h>
#include <glm.hpp>
#include <memory>
#include <vector>


//-------------------------------------------------------------------

class Camera;
class Shader;

struct DirectionalLight;
struct LightSpaceMatrixMappings;

//-------------------------------------------------------------------

struct ProjectionTracker
{
	float XMin = std::numeric_limits<float>::max();
	float XMax = std::numeric_limits<float>::min();
	float YMin = std::numeric_limits<float>::max();
	float YMax = std::numeric_limits<float>::min();
	float ZMin = std::numeric_limits<float>::max();
	float ZMax = std::numeric_limits<float>::min();
};

//-------------------------------------------------------------------

struct LightSpaceMatrixMappings
{
	LightSpaceMatrixMappings() {}
	LightSpaceMatrixMappings(glm::mat4 InProjection, glm::mat4 InViewMatrix, glm::vec3 InQuadrantCenter)
		: LightSpaceProjection(InProjection)
		, LightSpaceViewMatrix(InViewMatrix)
		, QuadrantCenterPosition(InQuadrantCenter)
	{

	}
	glm::mat4 LightSpaceProjection;
	glm::mat4 LightSpaceViewMatrix;
	glm::vec3 QuadrantCenterPosition;
};

//-------------------------------------------------------------------

class CascadingShadowMapHelper
{
public:
	static std::vector<glm::vec4>GetFrustrumCornersInWorldSpace(const glm::mat4& InProj, const glm::mat4& InView);
	static glm::vec3 GetFrustrumLightViewPosition(const std::vector<glm::vec4>& InCorners);
	static ProjectionTracker CreateProjectionTracker();
	static void AdjustProjectionTrackerToView(const glm::mat4& InLightView, ProjectionTracker& InTracker, const std::vector<glm::vec4>& InCorners);
	static glm::mat4 GetFrustrumLightProjection(ProjectionTracker& InTracker);
	static glm::mat4 GetFrustrumLightView(const glm::vec3& LightViewPosition, const glm::vec3& LightDirection, const glm::vec3& UpDirection);
	static LightSpaceMatrixMappings MakeCascadingLightSpaceMatrix(Camera* Perspective, const glm::vec3& InLightDirection, const float InNearPlane, const float InFarPlane);
	static std::vector<float>GetCascadingLevels(const float FarPlane);
	static void AddCascadingLevelsToShader(Shader* Shader, const float FarPlane);
	static void AddLightSpaceMatrixToShader(Shader* Shader, DirectionalLight* Light);
	static GLuint CreateMatricesUBO(const GLuint InSize);
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------


