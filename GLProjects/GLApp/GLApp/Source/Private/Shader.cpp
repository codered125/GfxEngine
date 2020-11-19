#pragma once

#include "Source/Public/Shader.h"

//-------------------------------------------------------------------

Shader::Shader(const GLchar *vertexPath, const GLchar *fragmentPath)
{

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
		std::cout << vertexPath << std::endl;
		std::cout << fragmentPath << std::endl;
	}

	glDeleteShader(vertShader);
	glDeleteShader(fragShader);

}

std::string Shader::RecursivelySearchForInclude(std::string CurrentShader)
{
	std::string OutputShader;
	std::ifstream CurrentShaderFile;
	CurrentShaderFile.exceptions(std::ifstream::badbit);
	try
	{

		//Open files
		CurrentShaderFile.open(CurrentShader);

		//Creates streams
		std::stringstream cShaderStream;

		//Read files buffer content into streams
		cShaderStream << CurrentShaderFile.rdbuf();

		//Close our handlers
		CurrentShaderFile.close();

		//Converting the stream to string
		OutputShader = cShaderStream.str();

	}
	catch (std::ifstream::failure e)
	{
		std::cout << "shader failed read" << std::endl;
	}
	return std::string();
}

//-------------------------------------------------------------------

void Shader::use()
{
	glUseProgram(this->shaderProgram);
}

//-------------------------------------------------------------------

void Shader::setFloat(const std::string& Accessor, float value)
{
	glUniform1f(glGetUniformLocation(shaderProgram, Accessor.c_str()), value);
}

//-------------------------------------------------------------------

void Shader::setVec3(const std::string& Accessor, glm::vec3 value)
{
	glUniform3fv(glGetUniformLocation(shaderProgram, Accessor.c_str()), 1, &value[0]);
}

//-------------------------------------------------------------------

void Shader::setVec4(const std::string &Accessor, glm::vec4  &value)
{
	glUniform4fv(glGetUniformLocation(shaderProgram, Accessor.c_str()), 1, &value[0]);
}

//-------------------------------------------------------------------

void Shader::setMat4(const std::string &Accessor, const glm::mat4 &value)
{
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, Accessor.c_str()), 1, GL_FALSE, &value[0][0]);
}

//-------------------------------------------------------------------

void Shader::setBool(const std::string &Accessor, bool value)
{
	glUniform1i(glGetUniformLocation(shaderProgram, Accessor.c_str()), (int)value);
}

//-------------------------------------------------------------------

void Shader::setInt(const std::string &Accessor, int value)
{
	glUniform1i(glGetUniformLocation(shaderProgram, Accessor.c_str()), value);
}

//-------------------------------------------------------------------

void Shader::SetSampler(const std::string & Accessor, GLuint* value, GLenum TextureType)
{
	const auto SamplerLocation = glGetUniformLocation(shaderProgram, Accessor.c_str());
	if (SamplerLocation != -1)
	{
		glActiveTexture(GL_TEXTURE0 + SamplerLocation);
		glBindTexture(TextureType, *value);
		glActiveTexture(GL_TEXTURE0);
	}

	/*const auto SamplerLocation = GetUniformLocation(Accessor);
	if (SamplerLocation != -1)
	{
		glActiveTexture(SamplerLocation);
		glBindTexture(TextureType, *value);
		glActiveTexture(GL_TEXTURE0);
	}*/
}

//-------------------------------------------------------------------

GLint Shader::GetUniformLocation(const std::string & Accessor)
{
	const auto SamplerLocation = glGetUniformLocation(shaderProgram, Accessor.c_str());
	return SamplerLocation;
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------



