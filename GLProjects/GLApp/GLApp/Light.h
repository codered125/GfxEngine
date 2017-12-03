#pragma once
#include <iostream>
#include <string.h>
#include <gl\glew.h>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
#include "Shader.h"

using namespace std;



enum LightType 
{
Point, Directional, Spot, nulls
};


struct Light
{
	Light(Shader * inShader, LightType inType)
	{
		ltype = inType;
		ShaderRef = inShader;
	};

	Shader * ShaderRef;
	float cutOff;
	float outerCutOff;
	float constant;
	float linear;
	float quadratic;

	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	
	LightType ltype;
	const GLchar accessor;

};


inline  Light PointLight(Shader * macShader) { return Light(macShader, LightType::Point); };

inline  Light SpotLight(Shader * macShader) { return Light(macShader, LightType::Spot); };

inline  Light DirectionalLight(Shader * macShader) { return Light(macShader, LightType::Directional); };
