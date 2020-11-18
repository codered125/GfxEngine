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
	GLenum WrapS;
	GLenum WrapT;
	GLenum MinFilter;
	GLenum MagFilter;
	GLenum TargetType;
	GLenum Format;
	GLenum InternalFormat;
	bool MipMap;
	bool MSAA;

};

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------


