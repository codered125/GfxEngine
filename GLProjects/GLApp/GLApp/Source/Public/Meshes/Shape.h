#pragma once

#include <GL\glew.h>
#include "Source/Public/Math.h"
#include <array>

//-------------------------------------------------------------------

class Shader;

//-------------------------------------------------------------------

class Shape
{
public:

	Shader* ThisShader;

	GLuint ShapeID;

protected:

	GLuint ShapeVAO;

	GLuint ShapeVBO;

	virtual void Draw(glm::mat4 InModel, glm::mat4 InFOV, glm::mat4 InView) = 0;

};

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
