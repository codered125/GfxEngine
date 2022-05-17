#include "Source/Public/Rendering/RenderTexture/RenderTexture.h"
#include "Source/Public/EngineDebugHelper.h"

#include <iostream>
#include <stb_image.h>

//-------------------------------------------------------------------

RenderTexture::RenderTexture()
{

}

//-------------------------------------------------------------------

RenderTexture::RenderTexture( GLuint InWidth, GLuint InHeight, GLuint InDepth, GLenum InTargetType, GLenum InInternalFormat,  GLenum InFormat, bool InMSAA, GLenum InMinFilter, GLenum InMagFilter, GLenum InWrap, const void* InPixels, GLenum InBufferType, std::function<void()> InPreParamInitalizerFunction)
{
	Params.TargetType = InTargetType;
	Params.Width = InWidth;
	Params.Depth = InDepth;
	Params.Height = InHeight;
	Params.Format = InFormat;
	Params.InternalFormat = InInternalFormat;
	Params.MSAA = InMSAA;
	Params.MinFilter = InMinFilter;
	Params.MagFilter = InMagFilter;
	Params.WrapR = InWrap;
	Params.WrapS = InWrap;
	Params.WrapT = InWrap;
	Params.BufferType = InBufferType;
	Params.Pixels = InPixels;
	if (InPreParamInitalizerFunction != nullptr)
	{
		Params.PreParamInitalizerFunction = InPreParamInitalizerFunction;
	}
	InitialiseRenderTexture(Params);

}

//-------------------------------------------------------------------

RenderTexture::RenderTexture(GLenum InTargetType, GLenum InInternalFormat, GLenum InFormat, const GLchar* InHDRPath, GLenum InBufferType, std::function<void()> InPreParamInitalizerFunction)
{
	Params.TargetType = InTargetType;
	Params.Format = InFormat;
	Params.InternalFormat = InInternalFormat;
	Params.MinFilter = GL_LINEAR;
	Params.MagFilter = GL_LINEAR;
	Params.WrapR = GL_CLAMP_TO_EDGE;
	Params.WrapS = GL_CLAMP_TO_EDGE;
	Params.WrapT = GL_CLAMP_TO_EDGE;

	stbi_set_flip_vertically_on_load(true);
	int nrComponents;
	float* data = stbi_loadf(InHDRPath, &Params.Width, &Params.Height, &nrComponents, 0);
	if (data)
	{
		Params.BufferType = InBufferType;
		Params.Pixels = data;
		if (InPreParamInitalizerFunction != nullptr)
		{
			Params.PreParamInitalizerFunction = InPreParamInitalizerFunction;
		}
		InitialiseRenderTexture(Params);
		stbi_image_free(data);
	}
	else
	{
		MoMessageLogger("Faled to load HDR Image");
	}
	stbi_set_flip_vertically_on_load(false);
}

//-------------------------------------------------------------------

void RenderTexture::InitialiseRenderTexture(RenderTextureParam& Params)
{
	Params.PreParamInitalizerFunction();

	glGenTextures(1, &Id);
	GLErrorCheck();
	glBindTexture(Params.TargetType, Id);
	GLErrorCheck();

	if (Params.MSAA)
	{
		GLuint AliasingCount = 4;
		glTexImage2DMultisample(Params.TargetType, AliasingCount, Params.InternalFormat, Params.Width, Params.Height, GL_TRUE);
		GLErrorCheck();
	}
	if(!Params.MSAA)
	{
		if (Params.TargetType == GL_TEXTURE_2D)
		{
			glTexImage2D(Params.TargetType, 0, Params.InternalFormat, Params.Width, Params.Height, 0, Params.Format, Params.BufferType, Params.Pixels);
			GLErrorCheck();
		}

		if (Params.TargetType == GL_TEXTURE_2D_ARRAY)
		{
			glTexImage3D(Params.TargetType, 0, Params.InternalFormat, Params.Width, Params.Height, Params.Depth, 0, Params.Format, Params.BufferType, Params.Pixels);
			GLErrorCheck();
		}
	}

	if (Params.Format == GL_DEPTH_COMPONENT || Params.Format == GL_DEPTH_COMPONENT32F)
	{
		float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glTexParameterfv(Params.TargetType, GL_TEXTURE_BORDER_COLOR, borderColor);
		GLErrorCheck();
	}
	if (Params.MipMap)
	{
		glGenerateMipmap(Params.TargetType);
		GLErrorCheck();
	}

	glTexParameteri(Params.TargetType, GL_TEXTURE_MIN_FILTER, Params.MinFilter);
	GLErrorCheck();
	glTexParameteri(Params.TargetType, GL_TEXTURE_MAG_FILTER, Params.MagFilter);
	GLErrorCheck();
	glTexParameteri(Params.TargetType, GL_TEXTURE_WRAP_S, Params.WrapS);
	GLErrorCheck();
	glTexParameteri(Params.TargetType, GL_TEXTURE_WRAP_T, Params.WrapT);
	GLErrorCheck();
	glTexParameteri(Params.TargetType, GL_TEXTURE_WRAP_R, Params.WrapR);
	GLErrorCheck();
	glBindTexture(Params.TargetType, 0);
}

//-------------------------------------------------------------------

GLuint& RenderTexture::GetID()
{
	return Id;
}

//-------------------------------------------------------------------

std::tuple<GLint, GLint> RenderTexture::GetWidthAndHeightOfTexture()
{
	return std::tuple<GLint, GLint>(Params.Width, Params.Height);
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------

