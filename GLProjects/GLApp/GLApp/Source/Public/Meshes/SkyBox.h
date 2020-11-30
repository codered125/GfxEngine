#pragma once

#include "Source/Public/Meshes/Shape.h"

#include <GL\glew.h>
#include <string>

//-------------------------------------------------------------------

class SkyBox : public Shape
{

public:


	SkyBox(Shader* SkyboxShader, std::string InPath, std::string InFormat);

	const GLfloat* GetVertices();

	virtual void Draw(glm::mat4 InModel, glm::mat4 InFOV, glm::mat4 InView) override;

	std::vector<const GLchar*> SkyboxFaces;	// Cubemap (Skybox)

	GLuint SkyboxTexture;

};

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------