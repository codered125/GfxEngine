#pragma once

//-------------------------------------------------------------------

#include "Source/Public/Lights/Light.h"
#include "Source/Public/Rendering/RenderingHelpers/CascadingShadowMapHelper.h"

//-------------------------------------------------------------------

#include <GL/glew.h>
#include <optional>
#include <vector>

//-------------------------------------------------------------------

struct DirectionalLight : public Light
{
public:

	DirectionalLight(Shader* inShader, std::string inAccessor);
	DirectionalLight(std::string inAccessor);
	const glm::mat4 GetLightSpaceProjection() const;
	const std::optional<glm::mat4> GetLightSpaceProjection(GLuint InIndex) const;
	const std::optional<glm::mat4> GetLightSpaceViewMatrix(GLuint InIndex) const;
	const GLuint GetNumberOfMatrixMappings() const;
	void AddLightSpaceViewMatrix(const LightSpaceMatrixMappings& InMatrixMapping);
	void AddLightSpaceViewMatrix(const LightSpaceMatrixMappings& InMatrixMapping, const GLuint InIndex);
	const std::optional<LightSpaceMatrixMappings> GetLightSpaceMatrix(GLuint InIndex) const;
	void ReserveMatrixMappings(GLuint ReserveNumber);
	virtual void SetupShader() override;
	virtual void SetupNewShader(Shader* InShader);
	
private:

	glm::mat4 LightSpaceProjection;
	glm::mat4 LightSpaceViewMatrix;
	std::vector<LightSpaceMatrixMappings> MatrixMappings;
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------

