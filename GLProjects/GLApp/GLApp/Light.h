#pragma once
#include <iostream>
#include <string.h>
#include <gl\glew.h>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>

struct Light
{
	std::string ShaderRef;
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
	Light(std::string inShaderStorageName)
	{
		ShaderRef = inShaderStorageName;
	};

};



#ifndef PointLight

#endif // !PointLight

#ifndef SpotLight

#endif // !PointLight

#ifndef DirectionalLight

#endif // !PointLight