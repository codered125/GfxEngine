#pragma once

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
	RenderTextureCubeMap(GLenum InTargetType, GLenum InInternalFormat, GLenum InFormat, const GLchar* InHDRPath);
	static std::vector<const GLchar*> LoadCubeMapFacesHelper(std::string InPath, std::string InFormat);

	GLuint& GetID();

private:
	GLuint Id;
	GLenum WrapS = GL_CLAMP_TO_EDGE;
	GLenum WrapT = GL_CLAMP_TO_EDGE;
	GLenum MinFilter = GL_LINEAR;
	GLenum MagFilter = GL_LINEAR;
	GLenum TargetType;
	GLenum Format;
	GLenum InternalFormat;
	bool MipMap;
	bool MSAA;
	std::vector<const GLchar*> Faces;

};

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------


