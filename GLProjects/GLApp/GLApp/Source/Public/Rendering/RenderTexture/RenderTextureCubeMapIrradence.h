#pragma once

#include "Source/Public/Rendering/RenderTexture/RenderTextureCubeMap.h"

//-------------------------------------------------------------------

class SceneRenderTarget;

//-------------------------------------------------------------------

class RenderTextureCubeMapIrradence : public RenderTextureCubeMap
{
public:

	RenderTextureCubeMapIrradence(){};
	RenderTextureCubeMapIrradence(GLenum InTargetType, GLenum InInternalFormat, GLenum InFormat, const GLchar* InHDRPath);
	RenderTextureCubeMap* GetUnConvolutedRenderTexture();

protected:

	RenderTextureCubeMap* HDRRenderTexture;
	RenderTextureCubeMap* UnConvolutedMap;
	SceneRenderTarget* IrrandenceRenderBuffer;
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------


