#pragma once

#include "Source/Public/Mesh.h"
#include "Source/Public/Shader.h"

//-------------------------------------------------------------------

Mesh::Mesh(std::vector<Vertex> inVertices, std::vector<GLuint> inIndices, std::vector<Texture> inTextures)
{
	vertices = inVertices;
	indices = inIndices;
	textures = inTextures;
	SetupMesh();
}

//-------------------------------------------------------------------

void Mesh::Draw(Shader * shader)
{
	GLuint diffuseNr = 1;
	GLuint specularNR = 1;
	GLuint normalNR = 1;
	GLuint heightNR = 1;
	GLuint roughnessNR = 1;
	GLuint metallic = 1;
	GLuint ao = 1;

	for (GLuint i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);

		std::stringstream ss;
		std::string number;
		std::string name = textures[i].type;

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
		glUniform1i(glGetUniformLocation(shader->shaderProgram, name.c_str()), i);
		glBindTexture(GL_TEXTURE_2D, textures[i].id);

	}

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	for (GLuint i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

//-------------------------------------------------------------------

void Mesh::SetupMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);


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

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------