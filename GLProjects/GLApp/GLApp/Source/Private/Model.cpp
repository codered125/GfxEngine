#pragma once

#include "Source/Public/Model.h"

#include <SOIL2/src\SOIL2\SOIL2.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

//-------------------------------------------------------------------

Model::Model(GLchar* inpath, Shader* InShader)
{
	ThisShader = InShader;
	this->loadModel(inpath);
}

//-------------------------------------------------------------------

void Model::Draw(Shader *shader)
{
	Shape::Draw(shader);

	for (GLuint i = 0; i < this->meshes.size(); i++)
	{
		this->meshes[i].Draw(shader, DrawType);
	}
}

//-------------------------------------------------------------------

void Model::loadModel(std::string path)
{
	Assimp::Importer importer;
	const aiScene * scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_FlipUVs);

	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "Failed Assimp load" << importer.GetErrorString() << std::endl;
		return;
	}
	//Because directories end with '/'
	this->directory = path.substr(0, path.find_last_of('/'));
	this->proccessNode(scene->mRootNode, scene);

}

//-------------------------------------------------------------------

void Model::proccessNode(aiNode * node, const aiScene * scene)
{
	for (GLuint i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh * mesh = scene->mMeshes[node->mMeshes[i]];
		this->meshes.push_back(this->proccesMesh(mesh, scene));
	}

	for (GLuint i = 0; i < node->mNumChildren; i++)
	{
		this->proccessNode(node->mChildren[i], scene);
	}
}

//-------------------------------------------------------------------

Mesh Model::proccesMesh(aiMesh * mesh, const aiScene * scene)
{
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;

	for (GLuint i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		glm::vec3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;

		if (mesh->HasNormals())
		{
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.Normal = vector;
		}

		if (mesh->mTextureCoords[0])
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoord = vec;
		}

		else
		{
			vertex.TexCoord = glm::vec2(0.0f, 0.0f);
		}

		//tangents
		if (mesh->mTangents != nullptr)
		{
			vector.x = mesh->mTangents[i].x;
			vector.y = mesh->mTangents[i].y;
			vector.z = mesh->mTangents[i].z;
			vertex.Tangent = vector;
		}
		// bitangent
		if (mesh->mBitangents != nullptr) {
			vector.x = mesh->mBitangents[i].x;
			vector.y = mesh->mBitangents[i].y;
			vector.z = mesh->mBitangents[i].z;
			vertex.Bitanget = vector;
		}
		vertices.push_back(vertex);

	}

	for (GLuint i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (GLuint j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);

		}

	}
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial * material = scene->mMaterials[mesh->mMaterialIndex];

		std::vector<Texture> diffuseMaps = this->loadMaterialTextures(material, aiTextureType_DIFFUSE, "material.texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		//vector<Texture> specularMaps = this->loadMaterialTextures(material, aiTextureType_SPECULAR, "material.texture_specular");
		std::vector<Texture> metalicMaps = this->loadMaterialTextures(material, aiTextureType_SPECULAR, "material.texture_metallic");
		textures.insert(textures.end(), metalicMaps.begin(), metalicMaps.end());

		std::vector<Texture> normalMaps = this->loadMaterialTextures(material, aiTextureType_HEIGHT, "material.texture_normal");
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

		std::vector<Texture> roughnessMaps = this->loadMaterialTextures(material, aiTextureType_SHININESS, "material.texture_roughness");
		textures.insert(textures.end(), roughnessMaps.begin(), roughnessMaps.end());

		std::vector<Texture> aoMaps = this->loadMaterialTextures(material, aiTextureType_AMBIENT, "material.texture_ao");
		textures.insert(textures.end(), aoMaps.begin(), aoMaps.end());
	}
	return Mesh(vertices, indices, textures);
}

//-------------------------------------------------------------------

std::vector<Texture> Model::loadMaterialTextures(aiMaterial * mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture> textures;
	for (GLuint i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);

		GLboolean skip = false;

		for (GLuint j = 0; j < textures_loaded.size(); j++)
		{
			if (textures_loaded[j].path == str)
			{
				textures.push_back(textures_loaded[j]);
				skip = true;
				break;
			}
		}

		if (!skip)
		{
			Texture texture;
			texture.id = TextureFromFile(str.C_Str(), this->directory);

			texture.type = typeName;

			texture.path = str;
			textures.push_back(texture);
			this->textures_loaded.push_back(texture);

		}
	}
	return textures;
}

//-------------------------------------------------------------------

GLint Model::TextureFromFile(const char * path, std::string directory)
{
	std::string filename = std::string(path);
	filename = directory + '/' + filename;
	GLuint textureID;
	glGenTextures(1, &textureID);

	int width, height;
	unsigned char * image = SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	SOIL_free_image_data(image);

	return textureID;
}

void Model::Draw(glm::mat4 InModel, glm::mat4 InFOV, glm::mat4 InView)
{
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------


