#pragma once

#include "Source/Public/Rendering/RenderTexture/RenderTextureCubeMap.h"

//-------------------------------------------------------------------

class SceneRenderTarget;
class RenderTexture;

//-------------------------------------------------------------------

class RenderTextureCubeMapIrradence 
{
public:

	RenderTextureCubeMapIrradence(){};
	RenderTextureCubeMapIrradence(GLenum InTargetType, GLenum InInternalFormat, GLenum InFormat, const GLchar* InHDRPath);
	RenderTextureCubeMap* GetUnConvolutedRenderTexture();
	RenderTextureCubeMap* GetPrefilteredEnvironmentMap();
	RenderTextureCubeMap* GetIrradenceDiffuse();
	RenderTextureCubeMap* GetIrradenceSpecular();
	virtual ~RenderTextureCubeMapIrradence();
protected:

	RenderTexture* HDRRenderTexture;
	RenderTextureCubeMap* UnConvolutedMap;
	RenderTextureCubeMap* PreFilteredEnvironmentMap;
	RenderTextureCubeMap* IrradenceDiffuse;
	RenderTextureCubeMap* IrradenceSpecular;
	SceneRenderTarget* IrrandenceRenderBuffer;
	RenderTextureCubeMapParam Params;
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------


