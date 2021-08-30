#pragma once

#include <gl/glew.h>

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
	bool MipMap;
	bool MSAA;
};


//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------