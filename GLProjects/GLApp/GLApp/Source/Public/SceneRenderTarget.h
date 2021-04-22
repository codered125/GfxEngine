#pragma once

#include <gl/glew.h>
#include <vector>

#include "Source/Public/RenderTexture.h"

//-------------------------------------------------------------------

class RenderTexture;

//-------------------------------------------------------------------

class SceneRenderTarget
{
public:
	SceneRenderTarget( GLuint InWidth, GLuint InHeight, GLenum InTargetType,  GLenum InInternalFormat, GLenum InFormat, GLuint InNrColourAttachments = 1, bool InMakeDepth = false, bool InMSAA = false, bool InRbo = false);
	GLuint& GetID();
	RenderTexture* GetColourAttachmentByIndex(GLint Index);
	RenderTexture* GetDepthTexture();
	void ResizeRenderTarget(GLint InWidth, GLint InHeight);

private:

	GLuint Id;
	GLint Height;
	GLint Width;
	GLuint Rbo;


	GLenum TargetType;

	//This has been Texture2D 99% of the time
	GLenum Format;
	GLenum InternalFormat;
	std::vector<RenderTexture> ColourAttachments;
	RenderTexture Depth;
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------


