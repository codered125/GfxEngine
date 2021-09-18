#include "Source/Public/Rendering/RenderTarget/SceneRenderTarget.h"
#include "Source/Public/EngineDebugHelper.h"

#include <iostream>

//-------------------------------------------------------------------

SceneRenderTarget::SceneRenderTarget( GLuint InWidth, GLuint InHeight, GLenum InTargetType, GLenum InInternalFormat, GLenum InFormat, GLuint InNrColourAttachments, bool InMakeDepth, bool InMSAA,bool InRbo)
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
		GLErrorCheck();
	}

	if (InNrColourAttachments > 1)
	{
		glDrawBuffers(InNrColourAttachments, AttachmentEnums);
	}
	if (InRbo)
	{
		glGenRenderbuffers(1, &Rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, Rbo);
		if (InMSAA)
		{
			glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, InWidth, InHeight);
		}
		else
		{
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, InWidth, InHeight);
		}

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, Rbo);
	}

	if (InMakeDepth)
	{
		Depth = RenderTexture(InWidth, InHeight, InTargetType, InInternalFormat, Format);
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
	return (GLint)ColourAttachments.size() > Index ? &ColourAttachments[Index] : nullptr;
}

//-------------------------------------------------------------------

RenderTexture* SceneRenderTarget::GetDepthTexture()
{
	return &Depth;
}

//-------------------------------------------------------------------

void SceneRenderTarget::ResizeRenderTarget(GLint InWidth, GLint InHeight, GLenum InRBOType)
{
	glBindFramebuffer(GL_FRAMEBUFFER, Id);
	glBindRenderbuffer(GL_RENDERBUFFER, Rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, InRBOType, InWidth, InHeight);

	Height = InHeight;
	Width = InWidth;
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------

