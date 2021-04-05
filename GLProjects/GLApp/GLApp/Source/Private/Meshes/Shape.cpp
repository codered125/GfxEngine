#pragma once 

#include "Source/Public/Meshes/Shape.h"

#include "Source/Public/Shader.h"

//-------------------------------------------------------------------

Shape::Shape()
{
	DrawType = Tesselation ? GL_PATCHES : GL_TRIANGLES;
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------