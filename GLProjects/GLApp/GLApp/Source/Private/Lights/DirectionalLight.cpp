#include "Source/Public/Lights/DirectionalLight.h"
#include "Source/Public/Shader.h"

//-------------------------------------------------------------------

DirectionalLight::DirectionalLight(Shader* inShader, std::string inAccessor) : Light(inShader, inAccessor)
{
	inShader->setVec3(pos, position);
	inShader->setVec3(dir, direction);
	inShader->setVec3(ambi, ambient);
	inShader->setVec3(diff, diffuse);
	inShader->setFloat(intense, intensity);
}

//-------------------------------------------------------------------

glm::mat4 DirectionalLight::GetLightSpaceProjection()
{
	auto Left = 40.0f;
	auto Top = 40.0f;
	return glm::ortho(-Left, Left, -Top, Top, -15.0f, 20.0f);
}

//-------------------------------------------------------------------

glm::mat4 DirectionalLight::GetLightSpaceViewMatrix()
{
	glm::vec3 SunDir(glm::vec3(-0.4f, -1.0f, 0.f));
	return glm::lookAt(-SunDir * 10.0f, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------