#pragma once


#ifndef SHADER_H
#define  Shader_H

//Loading headers
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <GL/glew.h>

class Shader
{
public:
	GLuint shaderProgram;

	Shader(const GLchar *vertexPath, const GLchar *fragmentPath) {

		//Gets shader source code from frag and vert path
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;

		//Ensures ifstream objects can throw exceptions

		vShaderFile.exceptions(std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::badbit);
		try
		{
			//Open files
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);

			//Creates streams
			std::stringstream vShaderStream, fShaderStream;

			//Read files buffer content into streams
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
		
			//Close our handlers
			vShaderFile.close();
			fShaderFile.close();

			//Converting the stream to string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure e)
		{
			std::cout << "shader failed read" << std::endl;
		}

		const GLchar *vShaderCode = vertexCode.c_str();
		const GLchar *fShaderCode = fragmentCode.c_str();

		//2. Compile Shaders


		GLint Success;
		GLchar infolog[512];

		//Create vertex shader
		GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertShader, 1, &vShaderCode, NULL);
		glCompileShader(vertShader);

		//Vertex Safety check
		glGetShaderiv(vertShader, GL_COMPILE_STATUS, &Success);
		if (!Success)
		{
			glGetShaderInfoLog(vertShader, 512, NULL, infolog);
			std::cout << "VertCompile Fail\n" << infolog << std::endl;
		}

		//Create Fragment shader
		GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragShader, 1, &fShaderCode, NULL);
		glCompileShader(fragShader);

		//Fragment Saftey check
		glGetShaderiv(fragShader, GL_COMPILE_STATUS, &Success);
		if (!Success)
		{
			glGetShaderInfoLog(fragShader, 512, NULL, infolog);
			std::cout << "frag  Fail\n" << infolog << std::endl;
		}


		this->shaderProgram = glCreateProgram();
		glAttachShader(this->shaderProgram, vertShader);
		glAttachShader(this->shaderProgram, fragShader);
		glLinkProgram(this->shaderProgram);

		glGetProgramiv(this->shaderProgram, GL_LINK_STATUS, &Success);
		if (!Success)
		{
			glGetProgramInfoLog(this->shaderProgram, 512, NULL, infolog);
			std::cout << "program  Fail\n" << infolog << std::endl;
		}

		glDeleteShader(vertShader);
		glDeleteShader(fragShader);

	}

	void use()
	{
		glUseProgram(this->shaderProgram);
	}

	void setFloat(const std::string Accessor, float value)
	{
		glUniform1f(glGetUniformLocation(shaderProgram, Accessor.c_str()), value);
	}


	void setVec3(const std::string Accessor, glm::vec3 value)
	{
		glUniform3fv(glGetUniformLocation(shaderProgram, Accessor.c_str()), 1, &value[0]);
	}

	void setVec4(const std::string &Accessor, glm::vec4  &value)
	{
		glUniform4fv(glGetUniformLocation(shaderProgram, Accessor.c_str()), 1, &value[0]);
	}

	void setMat4(const std::string &Accessor, const glm::mat4 &value)
	{
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, Accessor.c_str()), 1, GL_FALSE, &value[0][0]);
	}
};
#endif