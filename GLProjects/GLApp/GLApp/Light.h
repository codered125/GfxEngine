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
	Shader * ShaderRef = nullptr;
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
	
	LightType ltype = LightType::nulls;
	std::string accessor, pos, dir, ambi, diff, spec = "";

	Light(Shader * inShader, LightType inType, std::string inAccessor) : ltype(inType), ShaderRef(inShader)
	{
		accessor = inAccessor;
		pos = inAccessor + ".position";
		dir = inAccessor + ".direction";
		ambi = inAccessor + ".ambient";
		diff = inAccessor + ".diffuse";
		spec = inAccessor + ".specular";
	};

	void setUpShader()
	{
		ShaderRef->setVec3(pos, position);
		ShaderRef->setVec3(dir, direction);
		ShaderRef->setVec3(ambi, ambient);
		ShaderRef->setVec3(diff, diffuse);
		ShaderRef->setVec3(spec, specular);
	}

};


inline  Light PointLight(Shader * macShader, std::string macAccessor) { return Light(macShader, LightType::Point, macAccessor); };

inline  Light SpotLight(Shader * macShader, std::string macAccessor) { return Light(macShader, LightType::Spot, macAccessor); };

inline  Light DirectionalLight(Shader * macShader, std::string macAccessor) { return Light(macShader, LightType::Directional, macAccessor); };
