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



private:

	glm::mat4 LightSpaceProjection;
	glm::mat4 LightSpaceViewMatrix;
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------

