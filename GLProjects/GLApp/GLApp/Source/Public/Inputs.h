#pragma once

#include "Camera.h"

#include <gl/glew.h>
#include <GLFW/glfw3.h>

class Input 
{
public:

	 static void KeyCallback(GLFWwindow * window, int key, int scancode, int action, int mode, Camera * Cam, bool & KeyRef)
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, GL_TRUE);
		}

		if (key > 0 && key < 1024)
		{
			if (GLFW_PRESS == action)
			{
				KeyRef = true;
			}
			else if (GLFW_RELEASE == action)
			{
				KeyRef = false;
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

	 static void DoMovement(float deltaTime, Camera * Cam, bool(*Keys)[1024])
	{
		if (Keys[GLFW_KEY_W] || Keys[GLFW_KEY_UP]) Cam->ProcessKeyboard(EForward, deltaTime);
		if (Keys[GLFW_KEY_S] || Keys[GLFW_KEY_DOWN]) Cam->ProcessKeyboard(EBackward, deltaTime);
		if (Keys[GLFW_KEY_A] || Keys[GLFW_KEY_LEFT]) Cam->ProcessKeyboard(ELeft, deltaTime);
		if (Keys[GLFW_KEY_D] || Keys[GLFW_KEY_RIGHT]) Cam->ProcessKeyboard(ERight, deltaTime);

		/*
		if (keyboardlockout > 0)return;
		if (Keys[GLFW_KEY_1]) togglePostProcessEffects(1);
		if (Keys[GLFW_KEY_2]) togglePostProcessEffects(2);
		if (Keys[GLFW_KEY_3]) togglePostProcessEffects(3);
		if (Keys[GLFW_KEY_4]) togglePostProcessEffects(4);
		if (Keys[GLFW_KEY_5]) togglePostProcessEffects(5);
		if (Keys[GLFW_KEY_6]) togglePostProcessEffects(6);
		*/
	}

};