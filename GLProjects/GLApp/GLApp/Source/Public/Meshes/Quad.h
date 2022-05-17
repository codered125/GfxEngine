#pragma once
#include "Source/Public/Meshes/Shape.h"

#include <GL\glew.h>

//-------------------------------------------------------------------

struct PostProcessSettings;

//-------------------------------------------------------------------

class Quad : public Shape
{

public:

	Quad(Shader* InShader, PostProcessSettings* InPPS, bool InNDC);
	Quad(Shader* InShader);

	virtual void Draw(glm::mat4 InModel, glm::mat4 InFOV, glm::mat4 InView) override;
	virtual void Draw(glm::mat4 InModel, glm::mat4 InFOV, glm::mat4 InView, GLuint* Sampler);
	virtual void Draw(Shader* InShader);
	virtual void DrawTrianglesStrip(Shader* InShader);

protected:

	std::shared_ptr<PostProcessSettings> ThisPPS;

};

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
