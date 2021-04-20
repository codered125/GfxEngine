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
	stbi_set_flip_vertically_on_load(false);
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
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	stbi_set_flip_vertically_on_load(false);
}

//-------------------------------------------------------------------

RenderTextureCubeMap::RenderTextureCubeMap(GLenum InTargetType, GLenum InInternalFormat, GLenum InFormat, const GLchar* InHDRPath)
	: TargetType(InTargetType)
	, Format(InFormat)
	, InternalFormat(InInternalFormat)
{
	stbi_set_flip_vertically_on_load(true);
	int width, height, nrComponents;
	float* data = stbi_loadf(InHDRPath, &width, &height, &nrComponents, 0);
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
		MoMessageLogger("Faled to load HDR Image");
	}
	stbi_set_flip_vertically_on_load(false);
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
//-------------------------------------------------------------------
//-------------------------------------------------------------------
