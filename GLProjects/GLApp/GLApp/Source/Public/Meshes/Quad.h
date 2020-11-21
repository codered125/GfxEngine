#pragma once
#include "Source/Public/Meshes/Shape.h"

#include <GL\glew.h>

//-------------------------------------------------------------------

class Quad : public Shape
{

public:

	static const GLfloat* GetVerticesForNDCPlane();

	static const GLfloat* GetVerticesForPlane();
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
