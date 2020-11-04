#pragma once
#include "Source/Public/Light.h"
#include "Source/Public/Shader.h"
#include "Source/Public/Math.h"

//-------------------------------------------------------------------


//#include <gtc\type_ptr.hpp>

//-------------------------------------------------------------------

Light::Light(Shader * inShader, LightType inType, std::string inAccessor) : ltype(inType), ShaderRef(inShader), accessor(inAccessor)
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

void Light::setUpShader()
{
	switch (ltype)
	{
	case Point:
		ShaderRef->setVec3(pos, position);
		ShaderRef->setVec3(ambi, ambient);
		ShaderRef->setVec3(diff, diffuse);
		ShaderRef->setVec3(spec, specular);
		ShaderRef->setFloat(cons, constant);
		ShaderRef->setFloat(lin, linear);
		ShaderRef->setFloat(quad, quadratic);
		ShaderRef->setFloat(intense, intensity);
		break;

	case Directional:
		ShaderRef->setVec3(dir, direction);
		ShaderRef->setVec3(ambi, ambient);
		ShaderRef->setVec3(diff, diffuse);
		ShaderRef->setVec3(spec, specular);
		ShaderRef->setFloat(intense, intensity);
		break;

	case Spot:
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
		break;
	}
}

//-------------------------------------------------------------------

Light Light::PointLight(Shader * macShader, std::string macAccessor)
{
	return Light(macShader, LightType::Point, macAccessor);
};

//-------------------------------------------------------------------

Light Light::SpotLight(Shader * macShader, std::string macAccessor)
{
	return Light(macShader, LightType::Spot, macAccessor);
};

//-------------------------------------------------------------------

Light Light::DirectionalLight(Shader * macShader, std::string macAccessor)
{
	return Light(macShader, LightType::Directional, macAccessor);
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------