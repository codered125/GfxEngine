#pragma once

#include <gl/glew.h>
#include <vector>

#include "Source/Public/Rendering/RenderTarget/RenderTargetParams.h"
#include "Source/Public/Rendering/RenderTexture/RenderTexture.h"

//-------------------------------------------------------------------

class RenderTexture;
struct RenderTargertParam;

//-------------------------------------------------------------------

class SceneRenderTarget
{
public:
	SceneRenderTarget( GLuint InWidth, GLuint InHeight, GLenum InTargetType,  GLenum InInternalFormat, GLenum InFormat, GLuint InNrColourAttachments = 1, bool InMakeDepth = false, bool InMSAA = false, bool InRbo = false);
	GLuint& GetID();
	RenderTexture* GetColourAttachmentByIndex(GLint Index);
	RenderTexture* GetDepthTexture();
	void ResizeRenderTarget(GLint InWidth, GLint InHeight, GLenum InRBOType);

protected:
	void InitialiseSceneRenderTarget(RenderTargertParam& Params);

private:

	GLuint Id;
	GLuint Rbo;
	std::vector<RenderTexture> ColourAttachments;
	RenderTexture Depth;
	RenderTargertParam Param;
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------


