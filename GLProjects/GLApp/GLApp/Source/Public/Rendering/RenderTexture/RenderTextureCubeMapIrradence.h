#pragma once

#include "Source/Public/Rendering/RenderTexture/RenderTextureCubeMap.h"

#include <memory>

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
	RenderTexture* GetBRDFLookUpTexture();
	RenderTexture* GetHDRRenderTexture();
	virtual ~RenderTextureCubeMapIrradence();
protected:

	std::unique_ptr<RenderTexture> HDRRenderTexture;
	std::unique_ptr <RenderTexture> BRDFLookUpTexture;
	std::unique_ptr <RenderTextureCubeMap> UnConvolutedMap;
	std::unique_ptr <RenderTextureCubeMap> PreFilteredEnvironmentMap;
	std::unique_ptr <RenderTextureCubeMap> IrradenceDiffuse;
	//RenderTextureCubeMap* IrradenceSpecular;
	std::unique_ptr <SceneRenderTarget> IrrandenceRenderBuffer;
	RenderTextureCubeMapParam Params;
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------


