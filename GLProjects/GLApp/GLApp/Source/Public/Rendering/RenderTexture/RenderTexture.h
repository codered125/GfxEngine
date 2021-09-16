#pragma once

#include "Source/Public/Rendering/RenderTexture/RenderTextureParams.h"

#include <gl/glew.h>
#include <vector>
#include <tuple>

//-------------------------------------------------------------------

class RenderTexture
{
public:
	RenderTexture();
	RenderTexture(GLuint InWidth, GLuint InHeight, GLenum InTargetType, GLenum InInternalFormat, GLenum InFormat, bool InMSAA = false, GLenum InMinFilter = GL_LINEAR, GLenum InMagFilter = GL_LINEAR);
	RenderTexture(GLuint InWidth, GLuint InHeight, GLenum InTargetType, GLenum InInternalFormat, GLenum InFormat, const void* InPixels, GLenum InBufferType = GL_UNSIGNED_BYTE, GLenum InMinFilter = GL_LINEAR, GLenum InMagFilter = GL_LINEAR);
	RenderTexture(GLenum InTargetType, GLenum InInternalFormat, GLenum InFormat, const GLchar* InHDRPath, GLenum InBufferType);

	GLuint& GetID();
	std::tuple<GLint, GLint> GetWidthAndHeightOfTexture();

protected:
	void InitialiseRenderTexture(RenderTextureParam& Params);

private:

	GLuint Id;
	RenderTextureParam Params;

};

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------


