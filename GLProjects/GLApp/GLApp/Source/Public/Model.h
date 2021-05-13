#pragma once

#include "Source/Public/Math.h"
#include "Source/Public/Mesh.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map> 

#include "Source/Public/Meshes/Shape.h"

//-------------------------------------------------------------------

class Shader;
class Importer;

enum aiTextureType;

struct aiScene;
struct aiMaterial;
struct aiMesh;
struct aiNode;

//-------------------------------------------------------------------

class Model : public Shape
{
public:
	Model(GLchar * inpath, Shader* InShader);

	void Draw(Shader *shader) override;

private:
	std::vector<Mesh> meshes;
	std::string directory;
	std::vector<Texture> textures_loaded = std::vector<Texture>({ 0 });

	void loadModel(std::string path);

	void proccessNode(aiNode * node, const aiScene * scene);

	Mesh proccesMesh(aiMesh * mesh, const aiScene * scene);
	
	std::vector<Texture> loadMaterialTextures(aiMaterial * mat, aiTextureType type, std::string typeName);

	GLint TextureFromFile(const char * path, std::string directory);

	virtual void Draw(glm::mat4 InModel, glm::mat4 InFOV, glm::mat4 InView) override;

	static Importer GetImporterSingleTon;

};

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------


