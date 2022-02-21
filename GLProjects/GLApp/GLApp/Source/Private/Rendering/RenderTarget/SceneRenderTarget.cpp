#include "Source/Public/Rendering/RenderTarget/SceneRenderTarget.h"
#include "Source/Public/EngineDebugHelper.h"

#include <iostream>

//-------------------------------------------------------------------

SceneRenderTarget::SceneRenderTarget( GLuint InWidth, GLuint InHeight, GLenum InTargetType, GLenum InInternalFormat, GLenum InFormat, GLuint InNrColourAttachments, bool InMakeDepth, bool InMSAA, bool InRbo)
{
	Param.Width = InWidth;
	Param.Height = InHeight;
	Param.TargetType = InTargetType;
	Param.InternalFormat = InInternalFormat;
	Param.Format = InFormat;
	Param.NrColourAttachments = InNrColourAttachments;
	Param.MakeDepth = InMakeDepth;
	Param.MakeMSAA = InMSAA;
	Param.MakeRbo = InRbo;
	InitialiseSceneRenderTarget(Param);

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

	Param.Height = InHeight;
	Param.Width = InWidth;
}

//-------------------------------------------------------------------

void SceneRenderTarget::InitialiseSceneRenderTarget(RenderTargertParam& Params)
{
	glGenFramebuffers(1, &Id);
	glBindFramebuffer(GL_FRAMEBUFFER, Id);

#pragma warning(suppress : 4834)
	ColourAttachments.empty();
	GLuint* AttachmentEnums = new GLuint[Params.NrColourAttachments];

	for (GLuint i = 0; i < Params.NrColourAttachments; i++)
	{
		RenderTexture RT(Params.Width, Params.Height, Params.TargetType, Params.InternalFormat, Params.Format, Params.MakeMSAA, GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE, NULL, GL_FLOAT);
		ColourAttachments.push_back(RT);
		AttachmentEnums[i] = GL_COLOR_ATTACHMENT0 + i;
		glFramebufferTexture2D(GL_FRAMEBUFFER, AttachmentEnums[i], Params.TargetType, RT.GetID(), 0);
		GLErrorCheck();
	}

	if (Params.NrColourAttachments > 1)
	{
		glDrawBuffers(Params.NrColourAttachments, AttachmentEnums);
	}
	if (Params.MakeRbo)
	{
		glGenRenderbuffers(1, &Rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, Rbo);
		if (Params.MakeMSAA)
		{
			glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, Params.Width, Params.Height);
		}
		else
		{
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, Params.Width, Params.Height);
		}

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, Rbo);
	}

	if (Params.MakeDepth)
	{
		Depth = RenderTexture(Params.Width, Params.Height, Params.TargetType, Params.InternalFormat, Params.Format, Params.MakeMSAA, GLenum(GL_NEAREST), GLenum(GL_NEAREST), GLenum(GL_CLAMP_TO_BORDER), NULL, GL_UNSIGNED_BYTE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, Params.TargetType, Depth.GetID(), 0);
	}

	auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "SceneRenderTarget not complete: " << fboStatus << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------

