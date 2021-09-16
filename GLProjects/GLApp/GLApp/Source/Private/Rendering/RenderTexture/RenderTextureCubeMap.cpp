#include "Source/Public/Rendering/RenderTexture/RenderTextureCubeMap.h"
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
{
	Params.TargetType = InTargetType;
	Params.Format = InFormat;
	Params.InternalFormat = InInternalFormat;
	Params.Faces = InFaces;

	glGenTextures(1, &Id);
	GLErrorCheck();
	glBindTexture(Params.TargetType, Id);
	GLErrorCheck();

	int imageWidth, imageHeight;
	unsigned char *image;
	stbi_set_flip_vertically_on_load(false);
	for (GLuint i = 0; i < Params.Faces.size(); i++)
	{
		image = SOIL_load_image(Params.Faces[i], &imageWidth, &imageHeight, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, Params.InternalFormat, imageWidth, imageHeight, 0, Params.Format, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
	}

	glTexParameteri(Params.TargetType, GL_TEXTURE_MIN_FILTER, Params.MinFilter);
	GLErrorCheck();
	glTexParameteri(Params.TargetType, GL_TEXTURE_MAG_FILTER, Params.MagFilter);
	GLErrorCheck();
	glTexParameteri(Params.TargetType, GL_TEXTURE_WRAP_S, Params.WrapS);
	GLErrorCheck();
	glTexParameteri(Params.TargetType, GL_TEXTURE_WRAP_T, Params.WrapT);
	GLErrorCheck();
	glBindTexture(Params.TargetType, 0);
	stbi_set_flip_vertically_on_load(false);
}

//-------------------------------------------------------------------

RenderTextureCubeMap::RenderTextureCubeMap(GLenum InTargetType, GLenum InInternalFormat, GLenum InFormat, GLint InWidth, GLint InHeight, bool InGenerateMipMaps)
{
	Params.TargetType = InTargetType;
	Params.Format = InFormat;
	Params.InternalFormat = InInternalFormat;
	Params.Width = InWidth;
	Params.Height = InHeight;
	Params.MipMap = InGenerateMipMaps;
	Params.MinFilter = InGenerateMipMaps? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR;
	Params.BufferType = GL_FLOAT;


	glGenTextures(1, &Id);
	glBindTexture(Params.TargetType, Id);
	for (GLuint i = 0; i < 6; ++i)
	{
		// note that we store each face with 16 bit floating point values
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, Params.InternalFormat, Params.Width, Params.Height, 0, Params.Format, Params.BufferType, nullptr);
	}
	glTexParameteri(Params.TargetType, GL_TEXTURE_WRAP_S, Params.WrapS);
	glTexParameteri(Params.TargetType, GL_TEXTURE_WRAP_T, Params.WrapT);
	glTexParameteri(Params.TargetType, GL_TEXTURE_WRAP_R, Params.WrapR);
	glTexParameteri(Params.TargetType, GL_TEXTURE_MIN_FILTER, Params.MinFilter);
	glTexParameteri(Params.TargetType, GL_TEXTURE_MAG_FILTER, Params.MagFilter);

	if (InGenerateMipMaps)
	{
		glGenerateMipmap(Params.TargetType);
	}
}

//-------------------------------------------------------------------

void RenderTextureCubeMap::LoadCubeMapFacesHelper(std::string InPath, std::string InFormat, std::vector<const GLchar*>& InArray)
{
	//Right, left, top, bottom, back, front

	std::vector<const GLchar*> OutCubeMapFaces;
	auto lenth = std::strlen(std::string(InPath + "posx" + InFormat).c_str()) + 10;

	char* RightRaw = new char[lenth]; char* LeftRaw = new char[lenth];	char* TopRaw = new char[lenth];
	char* BottomRaw = new char[lenth];	char* FrontRaw = new char[lenth]; 	char* BackRaw = new char[lenth];

	std::string Right = InPath + "posx" + InFormat;
	strcpy_s(RightRaw, lenth, Right.c_str());
	OutCubeMapFaces.push_back(RightRaw);

	std::string Left = InPath + "negx" + InFormat;
	strcpy_s(LeftRaw, lenth, Left.c_str());
	OutCubeMapFaces.push_back(LeftRaw);

	std::string Top = InPath + "posy" + InFormat;
	strcpy_s(TopRaw, lenth, Top.c_str());
	OutCubeMapFaces.push_back(TopRaw);

	std::string Bottom = InPath + "negy" + InFormat;
	strcpy_s(BottomRaw, lenth, Bottom.c_str());
	OutCubeMapFaces.push_back(BottomRaw);

	std::string Front = InPath + "posz" + InFormat;
	strcpy_s(FrontRaw, lenth, Front.c_str());
	OutCubeMapFaces.push_back(FrontRaw);

	std::string Back = InPath + "negz" + InFormat;
	strcpy_s(BackRaw, lenth, Back.c_str());
	OutCubeMapFaces.push_back(BackRaw);

	InArray.reserve(sizeof(OutCubeMapFaces));
	InArray = move(OutCubeMapFaces);
}

//-------------------------------------------------------------------

GLuint& RenderTextureCubeMap::GetID()
{
	return Id;
}

//-------------------------------------------------------------------

void RenderTextureCubeMap::InitialiseRenderTexture(RenderTextureCubeMapParam& InParams)
{

}

//-------------------------------------------------------------------

RenderTextureCubeMap::~RenderTextureCubeMap()
{
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
