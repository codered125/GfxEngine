#include "Source/Public/Lights/DirectionalLight.h"
#include "Source/Public/Math.h"
#include "Source/Public/Shader.h"

//-------------------------------------------------------------------

DirectionalLight::DirectionalLight(Shader* inShader, std::string inAccessor)
	: Light(inShader, inAccessor)
	, MatrixMappings()
{
}

//-------------------------------------------------------------------

DirectionalLight::DirectionalLight(std::string inAccessor)
	: Light(nullptr, inAccessor)
	, MatrixMappings()
{

}

//-------------------------------------------------------------------

const glm::mat4 DirectionalLight::GetLightSpaceProjection() const
{
	auto Left = 50.0f; auto Top = 50.0f;
	return MoMath::MoOrthographic(-Left, Left, -Top, Top, -25.0f, 150.0f);
}

//-------------------------------------------------------------------

const std::optional<glm::mat4> DirectionalLight::GetLightSpaceProjection(GLuint InIndex) const
{
	if (MatrixMappings.size() > InIndex)
	{
		return MatrixMappings[InIndex].LightSpaceProjection;
	}

	return std::optional<glm::mat4>();
}

//-------------------------------------------------------------------

const std::optional<glm::mat4> DirectionalLight::GetLightSpaceViewMatrix(GLuint InIndex) const
{
	if (MatrixMappings.size() > InIndex)
	{
		return MatrixMappings[InIndex].LightSpaceViewMatrix;
	}

	return std::optional<glm::mat4>();
}

//-------------------------------------------------------------------

const GLuint DirectionalLight::GetNumberOfMatrixMappings() const
{
	return MatrixMappings.size();
}

//-------------------------------------------------------------------

void DirectionalLight::AddLightSpaceViewMatrix(const LightSpaceMatrixMappings&  InMatrixMapping)
{
	MatrixMappings.push_back(InMatrixMapping);
}

//-------------------------------------------------------------------

void DirectionalLight::AddLightSpaceViewMatrix(const LightSpaceMatrixMappings& InMatrixMapping, const GLuint InIndex)
{
	if (MatrixMappings.size() > InIndex)
	{
		 MatrixMappings[InIndex] = InMatrixMapping;
	}
	else
	{
		AddLightSpaceViewMatrix(InMatrixMapping);
	}
}

//-------------------------------------------------------------------

const std::optional<LightSpaceMatrixMappings>DirectionalLight::GetLightSpaceMatrix(GLuint InIndex) const
{
	if (MatrixMappings.size() > InIndex)
	{
		return MatrixMappings[InIndex];
	}

	return std::optional<LightSpaceMatrixMappings>();
}

//-------------------------------------------------------------------

void DirectionalLight::ReserveMatrixMappings(GLuint ReserveNumber)
{
	MatrixMappings.empty();
	MatrixMappings.reserve(ReserveNumber);
}

//-------------------------------------------------------------------

void DirectionalLight::SetupShader()
{
	Light::SetupShader();
}

//-------------------------------------------------------------------

void DirectionalLight::SetupNewShader(Shader* InShader)
{
	ShaderRef = InShader;
	SetupShader();
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------