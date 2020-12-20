#pragma once

//-------------------------------------------------------------------

#include "Source/Public/Lights/Light.h"

//-------------------------------------------------------------------

struct DirectionalLight : public Light
{
public:

	DirectionalLight(Shader* inShader, std::string inAccessor);

	glm::mat4 GetLightSpaceProjection();
	//glm::mat4 SetLightSpaceProjection(glm::mat4 InProjection);

	glm::mat4 GetLightSpaceViewMatrix();
	//glm::mat4 SetLightSpaceViewMatrix(glm::mat4* InMatrix);

private:

	glm::mat4 LightSpaceProjection;
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------

