#pragma once

#include "Source/Public/RenderTextureCubeMap.h"

//-------------------------------------------------------------------

class SceneRenderTarget;

//-------------------------------------------------------------------

class RenderTextureCubeMapIrradenceSpecular : public RenderTextureCubeMap
{
public:

	RenderTextureCubeMapIrradenceSpecular(){};
	RenderTextureCubeMapIrradenceSpecular(GLenum InTargetType, GLenum InInternalFormat, GLenum InFormat, const GLchar* InHDRPath);

protected:

	RenderTextureCubeMap* PreFilteredEnvironmentMap;
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------


