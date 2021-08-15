#pragma once

#include "Source/Public/RenderTextureCubeMapIrradenceSpecular.h"

//-------------------------------------------------------------------

#include "Source/Public/Meshes/Cube.h"
#include "Source/Public/EngineDebugHelper.h"
#include "Source/Public/SceneRenderTarget.h"
#include "Source/Public/Shader.h"

//-------------------------------------------------------------------

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <iostream>
#include <SOIL2/src/SOIL2/SOIL2.h>
#include <stb_image.h>

//-------------------------------------------------------------------

RenderTextureCubeMapIrradenceSpecular::RenderTextureCubeMapIrradenceSpecular(GLenum InTargetType, GLenum InInternalFormat, GLenum InFormat, const GLchar* InHDRPath)
{
	PreFilteredEnvironmentMap = new RenderTextureCubeMap(GL_TEXTURE_CUBE_MAP, GL_RGB16F, GL_RGB, 128, 128, true);
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
