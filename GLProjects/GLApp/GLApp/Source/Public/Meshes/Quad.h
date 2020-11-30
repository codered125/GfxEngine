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

	virtual void Draw(glm::mat4 InModel, glm::mat4 InFOV, glm::mat4 InView) override;

protected:

	PostProcessSettings* ThisPPS;

};

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
