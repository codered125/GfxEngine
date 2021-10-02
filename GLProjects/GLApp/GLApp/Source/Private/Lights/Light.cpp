#pragma once
#include "Source/Public/Lights/Light.h"
#include "Source/Public/Shader.h"
#include "Source/Public/Math.h"

//-------------------------------------------------------------------

//#include <gtc\type_ptr.hpp>

//-------------------------------------------------------------------

Light::Light(Shader * inShader, std::string inAccessor) : ShaderRef(inShader), accessor(inAccessor)
{
	accessor = inAccessor;
	pos = inAccessor + ".position";
	dir = inAccessor + ".direction";
	ambi = inAccessor + ".ambient";
	diff = inAccessor + ".diffuse";
	spec = inAccessor + ".specular";
	cutff = inAccessor + ".cutOff";
	outCutOff = inAccessor + ".outerCutOff";
	cons = inAccessor + ".constant";
	lin = inAccessor + ".linear";
	quad = inAccessor + ".quadratic";
	intense = inAccessor + ".intensity";
};

//-------------------------------------------------------------------

void Light::SetupShader()
{
	ShaderRef->setVec3(pos, position);
	ShaderRef->setVec3(dir, direction);
	ShaderRef->setVec3(ambi, ambient);
	ShaderRef->setVec3(diff, diffuse);
	ShaderRef->setVec3(spec, specular);
	ShaderRef->setFloat(cons, constant);
	ShaderRef->setFloat(lin, linear);
	ShaderRef->setFloat(quad, quadratic);
	ShaderRef->setFloat(cutff, cutOff);
	ShaderRef->setFloat(outCutOff, outerCutOff);
	ShaderRef->setFloat(intense, intensity);
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------