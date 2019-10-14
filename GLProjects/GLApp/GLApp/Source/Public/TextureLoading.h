#pragma once


#include <GL/glew.h>
#include <vector>

//-------------------------------------------------------------------

class TextureLoading
{
public:
	static GLuint LoadTexture(GLchar *path);
	
	static void SetupDPMapTex(GLuint * depthMapFBO, GLuint *depthMap);

	static GLuint LoadCubemap(std::vector<const GLchar * > faces);
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------