#include "Source/Public/Meshes/SkyBox.h"
#include "Source/Public/Shader.h"
#include "Source/Public/RenderTextureCubeMap.h"
#include "Source/Public/TextureLoading.h"

//-------------------------------------------------------------------

namespace StaticVerts
{
	GLfloat vertices[108] = {
		// Positions 

		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f

	};
}

//-------------------------------------------------------------------

SkyBox::SkyBox(Shader* SkyboxShader, std::string InPath, std::string InFormat)
{
	// Cubemap (Skybox)
	//Right, left, top, bottom, back, front
	SkyboxFaces = RenderTextureCubeMap::LoadCubeMapFacesHelper(InPath, InFormat);
	SkyboxTexture = new RenderTextureCubeMap(GL_TEXTURE_CUBE_MAP, GL_RGB, GL_RGB, SkyboxFaces);
	ThisShader = SkyboxShader;
}

//-------------------------------------------------------------------

void SkyBox::Draw(glm::mat4 InModel, glm::mat4 InFOV, glm::mat4 InView)
{
	if (ThisShader)
	{
		ThisShader->use();
		glGenBuffers(1, &ShapeVBO);
		glBindBuffer(GL_ARRAY_BUFFER, ShapeVBO);
		glGenVertexArrays(1, &ShapeVAO);
		glBindVertexArray(ShapeVAO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(StaticVerts::vertices), StaticVerts::vertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);

		glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content
		glm::mat4 view = glm::mat4(glm::mat3(InView));	// Remove any translation component of the view matrix

		ThisShader->setMat4("view", view);
		ThisShader->setMat4("projection", InFOV);

		glBindVertexArray(ShapeVAO);
		glActiveTexture(GL_TEXTURE0);
 		glBindTexture(GL_TEXTURE_CUBE_MAP, SkyboxTexture->GetID());
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // Set depth function back to default
	}
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------