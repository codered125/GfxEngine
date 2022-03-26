#pragma once

#include <functional>
#include <gl/glew.h>
#include <vector>

//-------------------------------------------------------------------

struct RenderTextureParam
{
public:
	GLint Height;
	GLint Width;
	GLenum WrapS = GL_CLAMP_TO_EDGE;
	GLenum WrapT = GL_CLAMP_TO_EDGE;
	GLenum WrapR = GL_CLAMP_TO_EDGE;
	GLenum MinFilter = GL_LINEAR;
	GLenum MagFilter = GL_LINEAR;
	GLenum TargetType;
	GLenum Format;
	GLenum InternalFormat;
	GLenum BufferType;
	const void* Pixels;
	bool MipMap = true;
	bool MSAA = false;
	std::function<void()> PreParamInitalizerFunction = [] { return; };
};

//-------------------------------------------------------------------

struct RenderTextureCubeMapParam : public RenderTextureParam
{
public:

	std::vector<const GLchar*> Faces;
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------