#pragma once

#include "Source/Public/Math.h"
#include "Source/Public/Mesh.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map> 

//-------------------------------------------------------------------

struct aiScene;
struct aiMaterial;
struct aiMesh;
struct aiNode;
class Shader;
enum aiTextureType;


//-------------------------------------------------------------------

class Actor
{
public:
	Model(GLchar * inpath);

	void Draw(Shader *shader, float shine);

private:
	std::vector<Mesh> meshes;
	std::string directory;
	std::vector<Texture> textures_loaded = std::vector<Texture>({ 0 });

	void loadModel(std::string path);

	void proccessNode(aiNode * node, const aiScene * scene);

	Mesh proccesMesh(aiMesh * mesh, const aiScene * scene);
	
	std::vector<Texture> loadMaterialTextures(aiMaterial * mat, aiTextureType type, std::string typeName);

	GLint TextureFromFile(const char * path, std::string directory);

};

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------


