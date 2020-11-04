#pragma once
#include "Source/Public/GlfwInterface.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

//-------------------------------------------------------------------

GLFWwindow * GlfwInterface::DefineAndCreaateWindow(int& InAntiAliasingCount, const GLint& inHeight, const GLint& inWidth)
{
	glfwInit();

	//Setting the version of our glfw window. Different versions have things like immediate mode removed ( 3.3)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//Doesnt care for backwards compatibility
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//Cares for forwards
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	//Toggles Resizeability
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	glfwWindowHint(GLFW_SAMPLES, InAntiAliasingCount);
	glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
	glEnable(GL_MULTISAMPLE);

	//Creating window
	auto window = glfwCreateWindow(inWidth, inHeight, "Moses Playboy Mansion", nullptr, nullptr);
	if (window == nullptr)
	{
		//Safety check
		std::cout << "Failed to create Window" << std::endl;
		glfwTerminate();
	}
	return window;
}

void GlfwInterface::GetFramebufferSize(GLFWwindow* InWindow, int& InWidth, int& InHeight)
{
	//Going to get the real size and height of the screen and store it to our storage
	//helps with high density displays. Will use it when creating viewport
	glfwGetFramebufferSize(InWindow, &InWidth, &InHeight);
}

void GlfwInterface::SetInputCallbackFunctions(GLFWwindow* InWindow, GLFWkeyfun Keyfunc, GLFWscrollfun Scrollfunc, GLFWcursorposfun Cursorfunc)
{
	glfwMakeContextCurrent(InWindow);
	glfwSetKeyCallback(InWindow, Keyfunc);
	glfwSetCursorPosCallback(InWindow, Cursorfunc);
	glfwSetScrollCallback(InWindow, Scrollfunc);
	glfwSetInputMode(InWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

bool GlfwInterface::WindowShouldClose(GLFWwindow* InWindow)
{
	return glfwWindowShouldClose(InWindow);
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------

