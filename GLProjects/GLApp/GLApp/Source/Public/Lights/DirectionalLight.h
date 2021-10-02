#pragma once

//-------------------------------------------------------------------

#include "Source/Public/Lights/Light.h"

//-------------------------------------------------------------------

#include <optional>
#include <vector>

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

struct DirectionalLight : public Light
{
public:

	DirectionalLight(Shader* inShader, std::string inAccessor);
	const glm::mat4 GetLightSpaceProjection() const;
	const std::optional<glm::mat4> GetLightSpaceViewMatrix(unsigned int InIndex) const;
	const unsigned int GetNumberOfMatrixMappings() const;
	void AddLightSpaceViewMatrix(LightSpaceMatrixMappings InMatrixMapping);
	virtual void SetupShader() override;
	
private:

	glm::mat4 LightSpaceProjection;
	glm::mat4 LightSpaceViewMatrix;
	std::vector<LightSpaceMatrixMappings> MatrixMappings;
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------

