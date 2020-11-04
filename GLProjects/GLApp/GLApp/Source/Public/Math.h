#pragma once


//-------------------------------------------------------------------

#include <vector>
#include <GL/glew.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

//-------------------------------------------------------------------

class MoMath
{
public:
	static glm::vec3 MoLerp(glm::vec3& a, glm::vec3& b, float& t);

	static float MoDotProduct(glm::vec3& a, glm::vec3& b);

	static glm::vec3 MoCrossProduct(glm::vec3& a, glm::vec3& b);

	static float MoSaturate(float& T);

	static float MoSign(float T);
	
	static float MoClamp(float& T, float A, float B);

	static glm::vec3 MoNormalize(glm::vec3& T);
};

//-------------------------------------------------------------------



//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------