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
	float cutOff = glm::cos(glm::radians(7.5f));
	float outerCutOff = glm::cos(glm::radians(11.0f));
	float constant = 1.0f; // keep at one stops denominator dropping below 1 in the equation
	float linear = 0.0014; //multiplied with the distance and decreases intensity in a linear fashion
	float quadratic = 0.000007;

	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 ambient = glm::vec3(0.05f, 0.05f, 0.05f);
	glm::vec3 diffuse;
	glm::vec3 specular = glm::vec3(1.0f);

	LightType ltype = LightType::nulls;
	std::string accessor, pos, dir, ambi, diff, spec, cutff, outCutOff, cons, lin, quad = "";

	Light(Shader * inShader, LightType inType, std::string inAccessor) : ltype(inType), ShaderRef(inShader) , accessor(inAccessor)
	{
		accessor = inAccessor;
		pos = inAccessor + ".position";
		dir = inAccessor + ".direction";
		ambi = inAccessor + ".ambient";
		diff = inAccessor + ".diffuse";
		spec = inAccessor + ".specular";
		cutff = inAccessor + ".cutOff";
		outCutOff = inAccessor + ".outerCutOff";
		cons = inAccessor + ".constant";
		lin = inAccessor + ".linear";
		quad = inAccessor + ".quadratic";
	};

	void setUpShader()
	{

		switch (ltype)
		{
		case Point: 
			ShaderRef->setVec3(pos, position);
			ShaderRef->setVec3(ambi, ambient);
			ShaderRef->setVec3(diff, diffuse);
			ShaderRef->setVec3(spec, specular);
			ShaderRef->setFloat(cons, constant );
			ShaderRef->setFloat(lin, linear );
			ShaderRef->setFloat(quad, quadratic  );
			break;

		case Directional:
			ShaderRef->setVec3(dir, direction);
			ShaderRef->setVec3(ambi, ambient);
			ShaderRef->setVec3(diff, diffuse);
			ShaderRef->setVec3(spec, specular);
			break;

		case Spot:
			ShaderRef->setVec3(pos, position);
			ShaderRef->setVec3(dir, direction);
			ShaderRef->setVec3(ambi, ambient);
			ShaderRef->setVec3(diff, diffuse);
			ShaderRef->setVec3(spec, specular);
			ShaderRef->setFloat(cons, constant);
			ShaderRef->setFloat(lin, linear);
			ShaderRef->setFloat(quad, quadratic);
			ShaderRef->setFloat(cutff, cutOff);
			ShaderRef->setFloat(outCutOff, outerCutOff);
			break;

		}

	}

};


inline  Light PointLight(Shader * macShader, std::string macAccessor) { return Light(macShader, LightType::Point, macAccessor); };

inline  Light SpotLight(Shader * macShader, std::string macAccessor) { return Light(macShader, LightType::Spot, macAccessor); };

inline  Light DirectionalLight(Shader * macShader, std::string macAccessor) { return Light(macShader, LightType::Directional, macAccessor); };
