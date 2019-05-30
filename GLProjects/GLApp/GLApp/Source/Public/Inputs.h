#pragma once

#include "Camera.h"
#include "PostProcessing.h"

#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <map>

class Input 
{
public:

	static void InitializeInputMap(std::map<int, int> & InputMap)
	{
		InputMap[1] = { GLFW_KEY_1 };
		InputMap[2] = { GLFW_KEY_2 };
		InputMap[3] = { GLFW_KEY_3 };
		InputMap[4] = { GLFW_KEY_4 };
		InputMap[5] = { GLFW_KEY_5 };
		InputMap[6] = { GLFW_KEY_6 };
	};
	
	static void KeyCallback(GLFWwindow * window, int key, int scancode, int action, int mode, Camera * Cam, bool KeyRef[])
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, GL_TRUE);
		}

		if (key > 0 && key < 1024)
		{
			if (GLFW_PRESS == action)
			{
				KeyRef[key] = true;
			}
			else if (GLFW_RELEASE == action)
			{
				KeyRef[key] = false;
			}
		}
	}

	static void ScrollCallback(GLFWwindow * window, double xOffset, double yOffset, Camera * Cam)
	{
		Cam->ProessMouseSroll((GLfloat)yOffset);
	}

	static void MouseCallback(GLFWwindow * window, double xPos, double yPos, Camera * Cam, GLfloat & lastX, GLfloat & lastY, bool & firstMouse)
	{
		if (firstMouse)
		{
			lastX = (GLfloat)xPos;
			lastY = (GLfloat)yPos;
			firstMouse = false;
		}

		GLfloat xOffset = (GLfloat)xPos - lastX;
		GLfloat yOffset = lastY - (GLfloat)yPos; //this is reverse because opengl Y coords start from bottom left and go reverse

		lastX = (GLfloat)xPos;
		lastY = (GLfloat)yPos;
		Cam->ProcessMouseMovement(xOffset, yOffset);
	}

	static void DoMovement(float deltaTime, Camera * Cam, bool Keys[], GLfloat & KeyboardLockout, PostProcessing::PostProcessSettings * PPS , std::map<int, int> & InputMap)
	{
		if (Keys[GLFW_KEY_W] || Keys[GLFW_KEY_UP])
		{
			Cam->ProcessKeyboard(EForward, deltaTime);
		}
		if (Keys[GLFW_KEY_S] || Keys[GLFW_KEY_DOWN])
		{
			Cam->ProcessKeyboard(EBackward, deltaTime);
		}
		if (Keys[GLFW_KEY_A] || Keys[GLFW_KEY_LEFT])
		{
			Cam->ProcessKeyboard(ELeft, deltaTime);
		}
		if (Keys[GLFW_KEY_D] || Keys[GLFW_KEY_RIGHT])
		{
			Cam->ProcessKeyboard(ERight, deltaTime);
		}

		
		if (KeyboardLockout > 0)
		{
			return;
		}

		for (int i = 0; i < 7; ++i)
		{
			if (Keys[InputMap[i]])
			{
				PostProcessing::TogglePostProcessEffects(i, PPS, KeyboardLockout);
			}
		}		
	}
};