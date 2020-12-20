#include "Source/Public/Lights/PointLight.h"
#include "Source/Public/Shader.h"

//-------------------------------------------------------------------

PointLight::PointLight(Shader* inShader, std::string inAccessor) : Light(inShader, inAccessor)
{
	inShader->setVec3(pos, position);
	inShader->setVec3(dir, direction);
	inShader->setVec3(ambi, ambient);
	inShader->setVec3(diff, diffuse);
	inShader->setVec3(spec, specular);
	inShader->setFloat(intense, intensity);
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
