#include "main.h"

#include <iostream>
#include <thread>
#include <future>

//-------------------------------------------------------------------------------------

#include <SOIL2\src\SOIL2\SOIL2.h>

//-------------------------------------------------------------------------------------
#include "Source/Public/GlfwInterface.h"
#include "Source/Public/Inputs.h"
#include "Source/Public/Camera.h"
#include "Source/Public/PostProcessing.h"
#include "Source/Public/Light.h"
#include "Source/Public/Rendering/ForwardRenderer.h"
#include "Source/Public/Rendering/DefferedRenderer.h"

//-------------------------------------------------------------------------------------

#define DEFFERED 0
int main()
{
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
		MainRenderer = &DefferedRenderer(SCREEN_WIDTH, SCREEN_HEIGHT);
	}
	else
	{
		MainRenderer = &ForwardRenderer(SCREEN_WIDTH, SCREEN_HEIGHT);
	}

	Input::InitializeInputMap(InputMap);

	while (!GlfwInterface::WindowShouldClose(window))
	{
		//check for events/inputs
		glfwPollEvents();

		//INPUTS
		Tick();
		Input::DoMovement(deltaTime, MainRenderer->MainCamera, Keys, keyboardlockout, MainRenderer->MainPostProcessSetting, InputMap);

		MainRenderer->RenderLoop();
		
		
		//swap screen buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return EXIT_SUCCESS;
}

//-------------------------------------------------------------------

void DrawLights(Shader * lampShader, Camera* Perspective, Model* InModel)
{
	//lampShader->use();
	//Cube LightCube;
	//LightCube.Colour = glm::vec3(50, 50, 50);

	//glm::mat4 FOV = Camera::GetProjection(Perspective);
	//glm::mat4 View = Camera::GetViewMatrix(Perspective);
	//glm::mat4 Model = glm::mat4();
	//Model = glm::translate(Model, TheMostStaticVertices::SunPos); //pointLightPositions[i]);
	//Model = glm::scale(Model, glm::vec3(2.f));
	//LightCube.ThisShader = lampShader;
	//LightCube.Draw(Model, FOV, View);

	//for (GLuint i = 0; i < sizeof(TheMostStaticVertices::pointLightPositions); i++)
	//{
	//	Model = glm::mat4();
	//	Model = glm::translate(Model, TheMostStaticVertices::pointLightPositions[i]); //pointLightPositions[i]);
	//	Model = glm::scale(Model, glm::vec3(0.2f));
	//	LightCube.Colour = TheMostStaticVertices::pointLightColours[i];
	//	LightCube.Draw(Model, FOV, View);
	//}
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
	Input::KeyCallback(window, key, scancode, action, mode, MainRenderer->MainCamera, Keys);
}

//-------------------------------------------------------------------

void ScrollCallback(GLFWwindow * window, double xOffset, double yOffset)
{
	Input::ScrollCallback(window, xOffset, yOffset, MainRenderer->MainCamera);
}

//-------------------------------------------------------------------

void MouseCallback(GLFWwindow * window, double xPos, double yPos)
{
	Input::MouseCallback(window, xPos, yPos, MainRenderer->MainCamera, lastX, lastY, firstMouse);
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------