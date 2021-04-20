#pragma once

#include <gl/glew.h>
#include <glm.hpp>

//-------------------------------------------------------------------

struct GLFWwindow;

typedef void(*GLFWkeyfun)(GLFWwindow*, int, int, int, int);
typedef void(*GLFWscrollfun)(GLFWwindow*, double, double);
typedef void(*GLFWcursorposfun)(GLFWwindow*, double, double);
typedef int GLint;

class GlfwInterface
{
public:

	static GLFWwindow* DefineAndCreaateWindow(int& InAntiAliasingCount, const GLint& inHeight, const GLint& inWidth);
	static void GetFramebufferSize(GLFWwindow* InWindow, int& InWidth, int& InHeight);
	static void ResetScreen(glm::vec4 InClearColour, GLbitfield InThingsToClear = GL_NONE, GLbitfield InThingsToEnable = GL_NONE, GLbitfield InThingsToDisable = GL_NONE);
	static void SetInputCallbackFunctions(GLFWwindow* InWindow, GLFWkeyfun Keyfunc, GLFWscrollfun Scrollfunc, GLFWcursorposfun Cursorfunc);
	static bool WindowShouldClose(GLFWwindow* InWindow);

};

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------