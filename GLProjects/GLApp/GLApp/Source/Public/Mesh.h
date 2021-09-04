#pragma once

#include "Source/Public/Math.h"

//-------------------------------------------------------------------
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <assimp/types.h>

//-------------------------------------------------------------------

struct  Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoord;
	glm::vec3 Tangent;
	glm::vec3 Bitanget;
};

//-------------------------------------------------------------------

struct Texture
{
	GLuint	id;
	std::string type;
	std::string path;
};

//-------------------------------------------------------------------

class Shader;

//-------------------------------------------------------------------

class Mesh
{
public:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;

	Mesh(std::vector<Vertex> inVertices, std::vector<GLuint> inIndices, std::vector<Texture> inTextures);

	void Draw(Shader* shader, GLenum DrawType = GL_TRIANGLES);

private:
	GLuint VAO, VBO, EBO;

	void SetupMesh();
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------