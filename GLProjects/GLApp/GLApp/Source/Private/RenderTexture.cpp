#include "Source/Public/RenderTexture.h"

#include <iostream>

//-------------------------------------------------------------------

RenderTexture::RenderTexture()
{

}

RenderTexture::RenderTexture( GLuint InWidth, GLuint InHeight, GLenum InTargetType, GLenum InInternalFormat,  GLenum InFormat, bool InMSAA)
	: Height(InHeight)
	, Width (InWidth)
	, TargetType(InTargetType)
	, Format(InFormat)
	, InternalFormat(InInternalFormat)
{
	glGenTextures(1, &Id);
	std::cout << "RenderTexture Gentexture: " << glGetError() << std::endl;
	glBindTexture(TargetType, Id);
	std::cout << "RenderTexture glBindTexture: " << glGetError() << std::endl;

	if (InMSAA)
	{
		GLuint AliasingCount = 4;
		glTexImage2DMultisample(TargetType, AliasingCount, InternalFormat, Width, Height, GL_TRUE);
		std::cout << "RenderTexture glTexImage2DMultisample: " << glGetError() << std::endl;
	}
	else
	{
		glTexImage2D(TargetType, 0, InternalFormat, Width, Height, 0, Format, GL_FLOAT, NULL);
		std::cout << "RenderTexture glTexImage2D : " << glGetError() << std::endl;
	}
	
	
	if (Format == GL_DEPTH_COMPONENT)
	{
		WrapS = GL_CLAMP_TO_BORDER;
		WrapT = GL_CLAMP_TO_BORDER;
		MinFilter = GL_NEAREST;
		MagFilter = GL_NEAREST;
		float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
		std::cout << "RenderTexture glTexParameterfv: " << glGetError() << std::endl;
	}


	glTexParameteri(TargetType, GL_TEXTURE_MIN_FILTER, MinFilter);	
	std::cout << "RenderTexture MinFilter: " << glGetError() << std::endl;
	glTexParameteri(TargetType, GL_TEXTURE_MAG_FILTER, MagFilter);
	std::cout << "RenderTexture MagFilter: " << glGetError() << std::endl;
	glTexParameteri(TargetType, GL_TEXTURE_WRAP_S, WrapS);
	std::cout << "RenderTexture WrapS: " << glGetError() << std::endl;
	glTexParameteri(TargetType, GL_TEXTURE_WRAP_T, WrapT);
	std::cout << "RenderTexture WrapT: " << glGetError() << std::endl;
	

}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------

