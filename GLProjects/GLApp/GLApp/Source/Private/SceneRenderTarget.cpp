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

	//Create a colour buffer 
	ColourAttachments.empty();
	GLuint* AttachmentEnums = new GLuint[InNrColourAttachments];

	for (GLuint i = 0; i < InNrColourAttachments; i++)
	{
		RenderTexture RT(Width, Height, TargetType, InternalFormat, Format, InMSAA);
		ColourAttachments.push_back(RT);
		AttachmentEnums[i] = GL_COLOR_ATTACHMENT0 + i;
		glFramebufferTexture2D(GL_FRAMEBUFFER, AttachmentEnums[i], InTargetType, RT.Id, 0);
		std::cout << "SceneRenderTarget glFramebufferTexture2D Id " << i << " : " << glGetError() << std::endl;
	}
		glDrawBuffers(InNrColourAttachments, AttachmentEnums);


	if (InMakeDepth)
	{
		Depth = RenderTexture(4096, 4096, GL_TEXTURE_2D, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, Depth.Id, 0);
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

