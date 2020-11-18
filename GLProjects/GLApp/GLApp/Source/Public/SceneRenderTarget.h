#pragma once

#include <gl/glew.h>
#include <vector>

#include "Source/Public/RenderTexture.h"

//-------------------------------------------------------------------

//class RenderTexture;

//-------------------------------------------------------------------

class SceneRenderTarget
{
public:
	SceneRenderTarget( GLuint InWidth, GLuint InHeight, GLenum InTargetType,  GLenum InInternalFormat, GLenum InFormat, GLuint InNrColourAttachments = 1, bool InMakeDepth = false, bool InMSAA = false);
	GLuint Id;
	std::vector<RenderTexture> ColourAttachments;
private:

	GLint Height;
	GLint Width;

	GLenum TargetType;

	//This has been Texture2D 99% of the time
	GLenum Format;
	GLenum InternalFormat;

	RenderTexture Depth;
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------


