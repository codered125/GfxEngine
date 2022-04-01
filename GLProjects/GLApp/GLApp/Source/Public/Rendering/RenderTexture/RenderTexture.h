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
	RenderTexture(GLuint InWidth, GLuint InHeight, GLenum InTargetType, GLenum InInternalFormat, GLenum InFormat, bool InMSAA, GLenum InMinFilter, GLenum InMagFilter, GLenum InWrap, const void* InPixels, GLenum InBufferType, std::function<void()> InPreParamInitalizerFunction);
	RenderTexture(GLenum InTargetType, GLenum InInternalFormat, GLenum InFormat, const GLchar* InHDRPath, GLenum InBufferType, std::function<void()> InPreParamInitalizerFunction);

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


