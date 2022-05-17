#pragma once

#include "Source/Public/Meshes/Shape.h"

#include <GL\glew.h>

//-------------------------------------------------------------------

class Cube : public Shape
{

public:

	const float* GetVertices();

	glm::vec3 Colour;

	virtual void Draw(glm::mat4 InModel, glm::mat4 InFOV, glm::mat4 InView) override;
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
