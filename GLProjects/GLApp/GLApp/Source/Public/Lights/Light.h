#pragma once
#include <iostream>
#include <string.h>
#include <glm.hpp>

//-------------------------------------------------------------------

class Shader;

//-------------------------------------------------------------------

enum LightType
{
	Point, Directional, Spot, nulls
};

//-------------------------------------------------------------------

struct Light
{
	Shader * ShaderRef = nullptr;
	float cutOff = glm::cos(glm::radians(7.5f));
	float outerCutOff = glm::cos(glm::radians(11.0f));
	float constant = 1.0f; // keep at one stops denominator dropping below 1 in the equation
	float linear = 0.0014f; //multiplied with the distance and decreases intensity in a linear fashion
	float quadratic = 0.000007f;
	float intensity = 5.0f;

	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 ambient = glm::vec3(0.05f, 0.05f, 0.05f);
	glm::vec3 diffuse;
	glm::vec3 specular = glm::vec3(1.0f);

	LightType ltype = LightType::nulls;
	std::string accessor, pos, dir, ambi, diff, spec, cutff, outCutOff, cons, lin, quad , intense= "";

	Light(Shader * inShader, std::string inAccessor);

	void setUpShader();

};

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------

