#include "Source/Public/RenderTexture.h"
#include "Source/Public/EngineDebugHelper.h"

#include <iostream>

//-------------------------------------------------------------------

RenderTexture::RenderTexture()
{

}

//-------------------------------------------------------------------

RenderTexture::RenderTexture( GLuint InWidth, GLuint InHeight, GLenum InTargetType, GLenum InInternalFormat,  GLenum InFormat, bool InMSAA)
	: Height(InHeight)
	, Width (InWidth)
	, TargetType(InTargetType)
	, Format(InFormat)
	, InternalFormat(InInternalFormat)
{
	glGenTextures(1, &Id);
	GLErrorCheck();
	glBindTexture(TargetType, Id);
	GLErrorCheck();

	if (InMSAA)
	{
		GLuint AliasingCount = 4;
		glTexImage2DMultisample(TargetType, AliasingCount, InternalFormat, Width, Height, GL_TRUE);
		GLErrorCheck();
	}
	else
	{
		glTexImage2D(TargetType, 0, InternalFormat, Width, Height, 0, Format, GL_FLOAT, NULL);
		GLErrorCheck();
	}
	
	if (Format == GL_DEPTH_COMPONENT)
	{
		WrapS = GL_CLAMP_TO_BORDER;
		WrapT = GL_CLAMP_TO_BORDER;
		MinFilter = GL_NEAREST;
		MagFilter = GL_NEAREST;
		float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
		GLErrorCheck();
	}

	glTexParameteri(TargetType, GL_TEXTURE_MIN_FILTER, MinFilter);	
	GLErrorCheck();
	glTexParameteri(TargetType, GL_TEXTURE_MAG_FILTER, MagFilter);
	GLErrorCheck();
	glTexParameteri(TargetType, GL_TEXTURE_WRAP_S, WrapS);
	GLErrorCheck();
	glTexParameteri(TargetType, GL_TEXTURE_WRAP_T, WrapT);
	GLErrorCheck();
}

//-------------------------------------------------------------------

GLuint& RenderTexture::GetID()
{
	return Id;
}

//-------------------------------------------------------------------

std::tuple<GLint, GLint> RenderTexture::GetWidthAndHeightOfTexture()
{
	return std::tuple<GLint, GLint>(Width, Height);
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------

