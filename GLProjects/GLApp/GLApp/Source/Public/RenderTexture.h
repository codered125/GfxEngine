#pragma once

#include <gl/glew.h>
#include <vector>
#include <tuple>

//-------------------------------------------------------------------


//-------------------------------------------------------------------

class RenderTexture
{
public:
	RenderTexture();
	RenderTexture(GLuint InWidth, GLuint InHeight, GLenum InTargetType, GLenum InInternalFormat, GLenum InFormat, bool InMSAA = false, GLenum InMinFilter = GL_LINEAR, GLenum InMagFilter = GL_LINEAR);
	RenderTexture(GLuint InWidth, GLuint InHeight, GLenum InTargetType, GLenum InInternalFormat, GLenum InFormat, const void* InPixels, GLenum InBufferType = GL_UNSIGNED_BYTE, GLenum InMinFilter = GL_LINEAR, GLenum InMagFilter = GL_LINEAR);

	GLuint& GetID();
	std::tuple<GLint, GLint> GetWidthAndHeightOfTexture();

private:
	GLuint Id;
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
	bool MipMap;
	bool MSAA;

};

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------


