#include "Source/Public/RenderTextureCubeMap.h"
#include "Source/Public/EngineDebugHelper.h"

#include <iostream>
#include <SOIL2/src/SOIL2/SOIL2.h>
#include <stb_image.h>

//-------------------------------------------------------------------

RenderTextureCubeMap::RenderTextureCubeMap()
{

}

//-------------------------------------------------------------------

RenderTextureCubeMap::RenderTextureCubeMap(GLenum InTargetType, GLenum InInternalFormat, GLenum InFormat, std::vector<const GLchar*> InFaces)
	: TargetType(InTargetType)
	, Format(InFormat)
	, InternalFormat(InInternalFormat)
	, Faces(InFaces)
{
	glGenTextures(1, &Id);
	GLErrorCheck();
	glBindTexture(TargetType, Id);
	GLErrorCheck();

	int imageWidth, imageHeight;
	unsigned char *image;

	for (GLuint i = 0; i < Faces.size(); i++)
	{
		image = SOIL_load_image(Faces[i], &imageWidth, &imageHeight, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, InInternalFormat, imageWidth, imageHeight, 0, InFormat, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
	}

	glTexParameteri(TargetType, GL_TEXTURE_MIN_FILTER, MinFilter);
	GLErrorCheck();
	glTexParameteri(TargetType, GL_TEXTURE_MAG_FILTER, MagFilter);
	GLErrorCheck();
	glTexParameteri(TargetType, GL_TEXTURE_WRAP_S, WrapS);
	GLErrorCheck();
	glTexParameteri(TargetType, GL_TEXTURE_WRAP_T, WrapT);
	GLErrorCheck();
}

//-------------------------------------------------------------------

RenderTextureCubeMap::RenderTextureCubeMap(GLenum InTargetType, GLenum InInternalFormat, GLenum InFormat, const GLchar* InHDRPath)
	: TargetType(InTargetType)
	, Format(InFormat)
	, InternalFormat(InInternalFormat)
{
	stbi_set_flip_vertically_on_load(true);
	int width, height, nrComponents;
	float *data = stbi_loadf(InHDRPath, &width, &height, &nrComponents, 0);
	unsigned int hdrTexture;
	if (data)
	{
		glGenTextures(1, &Id);
		glBindTexture(TargetType, Id);
		glTexImage2D(TargetType, 0, InInternalFormat, width, height, 0, InFormat, GL_FLOAT, data);

		glTexParameteri(TargetType, GL_TEXTURE_WRAP_S, WrapT);
		glTexParameteri(TargetType, GL_TEXTURE_WRAP_T, WrapT);
		glTexParameteri(TargetType, GL_TEXTURE_MIN_FILTER, MinFilter);
		glTexParameteri(TargetType, GL_TEXTURE_MAG_FILTER, MagFilter);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Failed to load HDR image." << std::endl;
	}
}

//-------------------------------------------------------------------

std::vector<const GLchar*> RenderTextureCubeMap::LoadCubeMapFacesHelper(std::string InPath, std::string InFormat)
{
	//Right, left, top, bottom, back, front

	std::vector<const GLchar*> OutCubeMapFaces;
	static std::string Right = InPath + "posx" + InFormat;
	OutCubeMapFaces.push_back(Right.c_str());

	static std::string Left = InPath + "negx" + InFormat;
	OutCubeMapFaces.push_back(Left.c_str());

	static std::string Top = InPath + "posy" + InFormat;
	OutCubeMapFaces.push_back(Top.c_str());

	static std::string Bottom = InPath + "negy" + InFormat;
	OutCubeMapFaces.push_back(Bottom.c_str());

	static std::string Front = InPath + "posz" + InFormat;
	OutCubeMapFaces.push_back(Front.c_str());

	static std::string Back = InPath + "negz" + InFormat;
	OutCubeMapFaces.push_back(Back.c_str());
	return OutCubeMapFaces;
}

//-------------------------------------------------------------------

GLuint& RenderTextureCubeMap::GetID()
{
	return Id;
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
