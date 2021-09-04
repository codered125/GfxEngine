#pragma once

#include <GL\glew.h>
#include "Source/Public/Math.h"
#include <array>

//-------------------------------------------------------------------

class Shader;
#define GET_VARIABLE_NAME(Variable) (#Variable)

//-------------------------------------------------------------------

class Shape
{
public:

	Shape();
	Shape(std::string InActorName, Shader* InShader);
	virtual ~Shape();

	virtual void Draw(glm::mat4 InModel, glm::mat4 InFOV, glm::mat4 InView) = 0;
	virtual void Draw(Shader *shader) {};

	Shader* ThisShader;
	GLuint ShapeID;
	GLenum DrawType = GL_TRIANGLES;

protected:

	GLuint ShapeVAO;
	GLuint ShapeVBO;

	std::string ActorName;
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
