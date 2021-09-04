#pragma once 

#include "Source/Public/Meshes/Shape.h"

#include "Source/Public/Shader.h"

//-------------------------------------------------------------------

Shape::Shape()
	: DrawType ( Tesselation ? GL_PATCHES : GL_TRIANGLES)
{

}

//-------------------------------------------------------------------


Shape::Shape(std::string InActorName, Shader* InShader)
	: DrawType(Tesselation ? GL_PATCHES : GL_TRIANGLES)
	, ActorName(InActorName)
	, ThisShader(InShader)
{
}

//-------------------------------------------------------------------

Shape::~Shape()
{
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------