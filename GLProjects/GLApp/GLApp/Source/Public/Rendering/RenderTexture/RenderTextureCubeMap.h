#pragma once

#include "Source/Public/Rendering/RenderTexture/RenderTextureParams.h"
#include <gl/glew.h>
#include <vector>
#include <tuple>

//-------------------------------------------------------------------


//-------------------------------------------------------------------

class RenderTextureCubeMap
{
public:
	RenderTextureCubeMap();
	RenderTextureCubeMap(GLenum InTargetType, GLenum InInternalFormat, GLenum InFormat, std::vector<const GLchar*> InFaces);
	RenderTextureCubeMap(GLenum InTargetType, GLenum InInternalFormat, GLenum InFormat, GLint InWidth, GLint InHeight, bool InGenerateMipMaps);
	virtual ~RenderTextureCubeMap();
	
	static void LoadCubeMapFacesHelper(std::string InPath, std::string InFormat, std::vector<const GLchar*>& InArray);
	GLuint& GetID();

protected:

	void InitialiseRenderTexture(RenderTextureCubeMapParam& InParams);

	GLuint Id;
	RenderTextureCubeMapParam Params;

};

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------


