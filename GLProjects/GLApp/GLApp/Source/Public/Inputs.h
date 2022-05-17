#pragma once

//-------------------------------------------------------------------

#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <map>

//-------------------------------------------------------------------

class Camera; 
struct PostProcessSettings;

//-------------------------------------------------------------------

class Input 
{
public:

	static void InitializeInputMap(std::map<int, int> & InputMap);

	static void KeyCallback(GLFWwindow * window, int key, int scancode, int action, int mode, Camera * Cam, bool KeyRef[]);

	static void ScrollCallback(GLFWwindow * window, double xOffset, double yOffset, Camera * Cam);

	static void MouseCallback(GLFWwindow * window, double xPos, double yPos, Camera * Cam, float & lastX, float & lastY, bool & firstMouse);

	static void DoMovement(float deltaTime, Camera * Cam, bool Keys[], float & KeyboardLockout, PostProcessSettings * PPS, std::map<int, int> & InputMap);
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------