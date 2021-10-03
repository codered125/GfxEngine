#pragma once

#include <gl/glew.h>
#include <vector>

//-------------------------------------------------------------------

struct RenderTargertParam
{
public:

	GLint Height;
	GLint Width;
	GLenum Format;
	GLenum InternalFormat;
	GLenum TargetType;
	bool MakeDepth = false;
	bool MakeRbo = false;
	bool MakeMSAA = false;
	GLuint NrColourAttachments = 1;

};

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------