#include "main.h"

//-------------------------------------------------------------------------------------

#include "Source/Public/Camera.h"
#include "Source/Public/GlfwInterface.h"
#include "Source/Public/Inputs.h"
#include "Source/Public/Lights/Light.h"
#include "Source/Public/Model.h"
#include "Source/Public/PostProcessing.h"
#include "Source/Public/Rendering/DefferedRenderer.h"
#include "Source/Public/Rendering/ForwardRenderer.h"


//-------------------------------------------------------------------------------------

#include <future>
#include <iostream>
#include <SOIL2\src\SOIL2\SOIL2.h>
#include <thread>

//-------------------------------------------------------------------------------------

#define DEFFERED 01
int main()
{
	//Model::GetImporterSingleTon();

	//WindowSetup
	auto* window = GlfwInterface::DefineAndCreaateWindow(AliasingCount, height, width);
	if (window == nullptr) 
	{
		return EXIT_FAILURE;
	};
	GlfwInterface::GetFramebufferSize(window, SCREEN_WIDTH, SCREEN_HEIGHT);
	GlfwInterface::SetInputCallbackFunctions(window, KeyCallback, ScrollCallback, MouseCallback);
	glewExperimental = GL_TRUE;//Tells glew to use a modern approach to retrieving function pointers and extensions
	glewInit();
	
	//inline initiation and safety check
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initializes Glew" << std::endl;
		return EXIT_FAILURE;
	}

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	if (DEFFERED)
	{
		MainRenderer = std::make_unique<DefferedRenderer>(SCREEN_WIDTH, SCREEN_HEIGHT);
	}
	else
	{
		MainRenderer = std::make_unique<ForwardRenderer>(SCREEN_WIDTH, SCREEN_HEIGHT);
	}

	Input::InitializeInputMap(InputMap);

	while (!GlfwInterface::WindowShouldClose(window))
	{
		//check for events/inputs
		glfwPollEvents();

		//INPUTS
		Tick();
		Input::DoMovement(deltaTime, MainRenderer->GetMainCamera(), Keys, keyboardlockout, MainRenderer->MainPostProcessSetting, InputMap);

		MainRenderer->RenderLoop(static_cast<float>(glfwGetTime()));
				
		//swap screen buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return EXIT_SUCCESS;
}

//-------------------------------------------------------------------

void Tick()
{
	const auto currentFrame = (GLfloat)glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	keyboardlockout = keyboardlockout > 0 ? keyboardlockout - deltaTime : 0;
}

//-------------------------------------------------------------------

void KeyCallback(GLFWwindow * window, int key, int scancode, int action, int mode)
{
	Input::KeyCallback(window, key, scancode, action, mode, MainRenderer->GetMainCamera(), Keys);
}

//-------------------------------------------------------------------

void ScrollCallback(GLFWwindow * window, double xOffset, double yOffset)
{
	Input::ScrollCallback(window, xOffset, yOffset, MainRenderer->GetMainCamera());
}

//-------------------------------------------------------------------

void MouseCallback(GLFWwindow * window, double xPos, double yPos)
{
	Input::MouseCallback(window, xPos, yPos, MainRenderer->GetMainCamera(), lastX, lastY, firstMouse);
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------