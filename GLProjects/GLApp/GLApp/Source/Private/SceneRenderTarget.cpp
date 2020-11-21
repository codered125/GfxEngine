#include "Source/Public/SceneRenderTarget.h"

#include <iostream>

//-------------------------------------------------------------------

SceneRenderTarget::SceneRenderTarget( GLuint InWidth, GLuint InHeight, GLenum InTargetType, GLenum InInternalFormat, GLenum InFormat, GLuint InNrColourAttachments, bool InMakeDepth, bool InMSAA)
	: Width(InWidth)
	, Height(InHeight)
	, TargetType(InTargetType)
	, Format(InFormat)
	, InternalFormat(InInternalFormat)
{
	glGenFramebuffers(1, &Id);
	glBindFramebuffer(GL_FRAMEBUFFER, Id);

	ColourAttachments.empty();
	GLuint* AttachmentEnums = new GLuint[InNrColourAttachments];

	for (GLuint i = 0; i < InNrColourAttachments; i++)
	{
		RenderTexture RT(Width, Height, TargetType, InternalFormat, Format, InMSAA);
		ColourAttachments.push_back(RT);
		AttachmentEnums[i] = GL_COLOR_ATTACHMENT0 + i;
		glFramebufferTexture2D(GL_FRAMEBUFFER, AttachmentEnums[i], InTargetType, RT.GetID(), 0);
		std::cout << "SceneRenderTarget glFramebufferTexture2D Id " << i << " : " << glGetError() << std::endl;
	}

	if (InNrColourAttachments > 1)
	{
		glDrawBuffers(InNrColourAttachments, AttachmentEnums);
	}

	if (InMakeDepth)
	{
		Depth = RenderTexture(InWidth, InHeight, InTargetType, InInternalFormat, InInternalFormat);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, InTargetType, Depth.GetID(), 0);
	}

	auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "SceneRenderTarget not complete: " << fboStatus << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

//-------------------------------------------------------------------

GLuint& SceneRenderTarget::GetID()
{
	return Id;
}

//-------------------------------------------------------------------

RenderTexture* SceneRenderTarget::GetColourAttachmentByIndex(GLint Index)
{
	return ColourAttachments.size() > Index ? &ColourAttachments[Index] : nullptr;
}

//-------------------------------------------------------------------

RenderTexture* SceneRenderTarget::GetDepthTexture()
{
	return &Depth;
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------

