#pragma once


#include <GL/glew.h>
#include <vector>

//-------------------------------------------------------------------

class TextureLoading
{
public:
	static GLuint LoadTexture(const GLchar *path);
	
	static GLuint LoadCubemap(std::vector<const GLchar * > faces);
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------