#pragma once

//-------------------------------------------------------------------

#ifndef SHADER_H
#define  Shader_H

#include "Source/Public/Math.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


//-------------------------------------------------------------------

class Shader
{
public:
	GLuint shaderProgram;

	Shader() {};

	Shader(const GLchar *vertexPath, const GLchar *fragmentPath);

	static std::string ParseShaderForIncludes(const GLchar* CurrentShaderPath);

	void use();

	void setFloat(const std::string& Accessor, float value);

	void setVec3(const std::string& Accessor, glm::vec3 value);

	void setVec4(const std::string& Accessor, glm::vec4& value);

	void setMat4(const std::string& Accessor, const glm::mat4& value);

	void setBool(const std::string& Accessor, bool value);

	void setInt(const std::string& Accessor, int value);

	void SetSampler(const std::string& Accessor, GLuint* value, GLenum TextureType);

	GLint GetUniformLocation(const std::string& Accessor);
};

#endif

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------