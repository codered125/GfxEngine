#include "Source/Public/Lights/DirectionalLight.h"
#include "Source/Public/Math.h"
#include "Source/Public/Shader.h"

//-------------------------------------------------------------------

DirectionalLight::DirectionalLight(Shader* inShader, std::string inAccessor)
	: Light(inShader, inAccessor)
	, MatrixMappings()
{
	inShader->setVec3(pos, position);
	inShader->setVec3(dir, direction);
	inShader->setVec3(ambi, ambient);
	inShader->setVec3(diff, diffuse);
	inShader->setFloat(intense, intensity);
}

//-------------------------------------------------------------------

const glm::mat4 DirectionalLight::GetLightSpaceProjection() const
{
	auto Left = 40.0f; auto Top = 40.0f;
	return MoMath::MoOrthographic(-Left, Left, -Top, Top, -15.0f, 20.0f);
}

//-------------------------------------------------------------------

const std::optional<glm::mat4> DirectionalLight::GetLightSpaceViewMatrix(unsigned int InIndex) const
{
	if (MatrixMappings.size() > InIndex)
	{
		return MatrixMappings[InIndex].LightSpaceViewMatrix;
	}

	return std::optional<glm::mat4>();
}

//-------------------------------------------------------------------

const unsigned int DirectionalLight::GetNumberOfMatrixMappings() const
{
	return MatrixMappings.size();
}

//-------------------------------------------------------------------

void DirectionalLight::AddLightSpaceViewMatrix(LightSpaceMatrixMappings InMatrixMapping)
{
	MatrixMappings.push_back(InMatrixMapping);
}

//-------------------------------------------------------------------

void DirectionalLight::SetupShader()
{
	Light::SetupShader();
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------