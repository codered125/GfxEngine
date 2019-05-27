#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>



#include <GL/glew.h>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>


#include "Shader.h"

using namespace std;

struct  Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoord;
	glm::vec3 Tangent;
	glm::vec3 Bitanget;
};

struct Texture
{
	GLuint	id;
	string type;
	aiString path;
};

class Mesh
{
public:
	vector<Vertex> vertices;
	vector<GLuint> indices;
	vector<Texture> textures;

	Mesh(vector<Vertex> inVertices, vector<GLuint> inIndices, vector<Texture> inTextures)
	{
		this->vertices = inVertices;
		this->indices = inIndices;
		this->textures = inTextures;

		this->SetupMesh();
	}

	void Draw(Shader * shader, float Shine)
	{
		GLuint diffuseNr = 1;
		GLuint specularNR = 1;
		GLuint normalNR = 1;
		GLuint heightNR = 1;
		GLuint roughnessNR = 1;
		GLuint metallic = 1;
		GLuint ao = 1;	

		for (GLuint i = 0; i < this->textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);

			stringstream ss; 
			string number;
			string name = this->textures[i].type;
			
			if ("material.texture_diffuse" == name)
			{
				ss << diffuseNr++;
			}

			else if ("material.texture_specular" == name)
			{
				ss << specularNR++;
			}

			else if ("material.texture_normal" == name)
			{
				ss << normalNR++;
			}

			else if ("material.texture_height" == name)
			{
				ss << heightNR++;
			}
			else if ("material.texture_roughness" == name)
			{
				ss << roughnessNR++;
			}

			else if ("material.texture_ao" == name)
			{
				ss << ao++;
			}

			else if ("material.texture_metallic" == name)
			{
				ss << metallic++;
			}


			
			number = ss.str();
			//cout << (name + number) << endl;
			glUniform1i(glGetUniformLocation(shader->shaderProgram, name.c_str()), i);
			//glUniform1i(glGetUniformLocation(shader->shaderProgram, (name + number).c_str()), i);
			glBindTexture(GL_TEXTURE_2D, this->textures[i].id);

		}

		glUniform1f(glGetUniformLocation(shader->shaderProgram, "material.shininess"), Shine);

		glBindVertexArray(this->VAO);
		glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		for (GLuint i = 0; i < this->textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE + i);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}

private:
	GLuint VAO, VBO, EBO;

	void SetupMesh()
	{
		glGenVertexArrays(1, &this->VAO);
		glGenBuffers(1, &this->VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(this->VAO);
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);

		//Vertex Positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)0);
		

		//Vertex Normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, Normal));

		//Vertex texture coordinates
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, TexCoord));

		//Vertex tanget
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, Tangent));

		//Vertext bitangent
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, Bitanget));

		glBindVertexArray(0);

	}

};