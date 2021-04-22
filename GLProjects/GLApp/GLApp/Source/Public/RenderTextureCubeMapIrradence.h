#pragma once

#include "Source/Public/RenderTextureCubeMap.h"

//-------------------------------------------------------------------

class SceneRenderTarget;

//-------------------------------------------------------------------

class RenderTextureCubeMapIrradence : public RenderTextureCubeMap
{
public:

	RenderTextureCubeMapIrradence(){};
	RenderTextureCubeMapIrradence(GLenum InTargetType, GLenum InInternalFormat, GLenum InFormat, const GLchar* InHDRPath);

protected:

	RenderTextureCubeMap* HDRRenderTexture;
	RenderTextureCubeMap* UnConvolutedMap;
	SceneRenderTarget* IrrandenceRenderBuffer;

};

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------


