#pragma once

#include <gl/glew.h>
#include <vector>

//-------------------------------------------------------------------


//-------------------------------------------------------------------

class RenderTexture
{
public:
	RenderTexture();
	RenderTexture(GLuint InWidth, GLuint InHeight, GLenum InTargetType, GLenum InInternalFormat, GLenum InFormat, bool InMSAA = false);

	GLuint Id;
	GLint Height;
	GLint Width;
	GLenum WrapS = GL_CLAMP_TO_EDGE;
	GLenum WrapT = GL_CLAMP_TO_EDGE;
	GLenum MinFilter = GL_LINEAR;
	GLenum MagFilter = GL_LINEAR;
	GLenum TargetType;
	GLenum Format;
	GLenum InternalFormat;
	bool MipMap;
	bool MSAA;

};

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------


