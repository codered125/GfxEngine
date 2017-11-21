#include <iostream>
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL2\src\SOIL2\SOIL2.h>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
#include "Shader.h"
#include "Cube.h"
#include "Model.h"
#include "Camera.h"

const GLint width = 1200, height = 800;

void  GLFWInit();
int SCREEN_WIDTH, SCREEN_HEIGHT;

void KeyCallback(GLFWwindow  * window, int key, int scancode, int action, int mode);
void ScrollCallback(GLFWwindow * window, double xOffset, double yOffset);
void MouseCallback(GLFWwindow * window, double xPos, double yPos);
void DoMovement();
void Tick();

void initialiseLights(Shader * lightShader);
void DrawLights(Shader * lampShader);
Camera ourCamera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = width / 2.0f;
GLfloat lastY = height / 2.0f;
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
Cube base;
bool Keys[1024];
bool firstMouse = true;
glm::vec3 pointLightPositions[] =
{
	glm::vec3(0.7f, 0.2f, 2.0f),
	glm::vec3(2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f, 2.0f, -6.0f),
	glm::vec3(0.0f, 0.0f, -3.0f)
};
glm::vec3 pointLightColours[] =
{
	/*
	glm::vec3(0.3f, 0.3f, 0.3f),
	glm::vec3(0.0f, 1.0f, 0.0f),
	glm::vec3(1.0f, 0.0f, 0.0f),
	glm::vec3(0.0f, 0.0f, 1.0f)
	*/

	glm::vec3(1.0f, 1.0f, 1.0f),
	glm::vec3(1.0f, 1.0f, 1.0f),
	glm::vec3(1.0f, 1.0f, 1.0f),
	glm::vec3(1.0f, 1.0f, 1.0f)
};
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);


int main()
{

	GLFWInit();

	//Creating window

	GLFWwindow *  window = glfwCreateWindow(width, height, "Moses Playboy Mansion", nullptr, nullptr);

	//Going to get the real size and height of the screen and store it to our storage
	//helps with high density displays. Will use it when creating viewport

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	if (window == nullptr)
	{
		//Safety check
		std::cout << "Failed to create Window" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	//Makes current windows
	glfwMakeContextCurrent(window);


	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);
	glfwSetScrollCallback(window, ScrollCallback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//Tells glew to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;

	//inline initiation and safety check
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initializes Glew" << std::endl;
		return EXIT_FAILURE;
	}

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Shader shader("Shaders/modelLoading.vs", "Shaders/modelLoading.frag");
	Shader lampShader("Shaders/Lamp.vs", "Shaders/Lamp.frag");
	//Model ourModel("Models/Nanosuit/nanosuit.obj");
	Model ourModel("Models/Brough/Brough_Castle_Model_121k.obj");
	
	glm::mat4 FOV;
	FOV = glm::perspective(ourCamera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 1000.0f);

	while (!glfwWindowShouldClose(window))
	{
		Tick();
		//check for events/inputs
		glfwPollEvents();
		DoMovement();

		//RENDER
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shader.use();

		glm::mat4 view = ourCamera.GetViewMatrix();
		glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(FOV)); 
		glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));

		glm::mat4 model;
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		model = glm::rotate(model, -90.0f , glm::vec3( 1.0f, 0.0f, 0.0f));

		glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
		initialiseLights(&shader);
		ourModel.Draw(&shader);
		DrawLights(&lampShader);


		//swap screen buffers
		glfwSwapBuffers(window);

	}

	glfwTerminate();
	return EXIT_SUCCESS;
}

void  GLFWInit()
{
	glfwInit();

	//Setting the version of our glfw window. Different versions have things like immediate mode removed ( 3.3)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//Doesnt care for backwards compatibility
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//Cares for forwards
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	//Toggles Resizeability
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
}

void KeyCallback(GLFWwindow * window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)glfwSetWindowShouldClose(window, GL_TRUE);

	if (key > 0 && key < 1024)
	{
		if (GLFW_PRESS == action) Keys[key] = true;
		else if (GLFW_RELEASE == action)Keys[key] = false;
	}
}

void ScrollCallback(GLFWwindow * window, double xOffset, double yOffset)
{
	ourCamera.ProessMouseSroll(yOffset);
}

void MouseCallback(GLFWwindow * window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos; //this is reverse because opengl Y coords start from bottom left and go reverse

	lastX = xPos;
	lastY = yPos;
	ourCamera.ProcessMouseMovement(xOffset, yOffset);
}

void DoMovement()
{
	if (Keys[GLFW_KEY_W] || Keys[GLFW_KEY_UP]) ourCamera.ProcessKeyboard(EForward, deltaTime);
	if (Keys[GLFW_KEY_S] || Keys[GLFW_KEY_DOWN]) ourCamera.ProcessKeyboard(EBackward, deltaTime);
	if (Keys[GLFW_KEY_A] || Keys[GLFW_KEY_LEFT]) ourCamera.ProcessKeyboard(ELeft, deltaTime);
	if (Keys[GLFW_KEY_D] || Keys[GLFW_KEY_RIGHT]) ourCamera.ProcessKeyboard(ERight, deltaTime);

}

void Tick()
{
	//Calculate framedata
	GLfloat currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

}

void DrawLights(Shader * lampShader)
{
	lampShader->use();
	GLuint lightVAO, lightVBO;

	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	glGenBuffers(1, &lightVBO);
	glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(base.vertices), base.vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);


	glm::mat4 model;

	glm::mat4 FOV;
	FOV = glm::perspective(ourCamera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 1000.0f);
	glUniformMatrix4fv(glGetUniformLocation(lampShader->shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(FOV));

	glm::mat4 view;
	view = ourCamera.GetViewMatrix();
	glUniformMatrix4fv(glGetUniformLocation(lampShader->shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glBindVertexArray(lightVAO);
	for (GLuint i = 0; i < 4; i++)
	{
		model = glm::mat4();
		model = glm::translate(model, pointLightPositions[i]);
		model = glm::scale(model, glm::vec3(0.2f));
		glUniformMatrix4fv(glGetUniformLocation(lampShader->shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
		GLfloat convert[3] = { pointLightColours[i].x,pointLightColours[i].y,pointLightColours[i].z };
		glUniform3fv(glGetUniformLocation(lampShader->shaderProgram, "inColour"), 1, convert);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	glBindVertexArray(0);
}

void initialiseLights(Shader * lightShader)
{


	// Here we set all the uniforms for the 5/6 types of lights we have. We have to set them manually and index
	// the proper PointLight struct in the array to set each uniform variable. This can be done more code-friendly
	// by defining light types as classes and set their values in there, or by using a more efficient uniform approach
	// by using 'Uniform buffer objects', but that is something we discuss in the 'Advanced GLSL' tutorial.


	// Directional light
	glUniform3f(glGetUniformLocation(lightShader->shaderProgram, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
	glUniform3f(glGetUniformLocation(lightShader->shaderProgram, "dirLight.ambient"), 0.05f, 0.05f, 0.05f);
	glUniform3f(glGetUniformLocation(lightShader->shaderProgram, "dirLight.diffuse"), 0.4f, 0.4f, 0.4f);
	glUniform3f(glGetUniformLocation(lightShader->shaderProgram, "dirLight.specular"), 0.5f, 0.5f, 0.5f);

	// Point light 1
	glUniform3f(glGetUniformLocation(lightShader->shaderProgram, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
	glUniform3f(glGetUniformLocation(lightShader->shaderProgram, "pointLights[0].ambient"), 0.05f, 0.05f, 0.05f);
	glUniform3f(glGetUniformLocation(lightShader->shaderProgram, "pointLights[0].diffuse"), pointLightColours[0].x, pointLightColours[0].y, pointLightColours[0].z);
	glUniform3f(glGetUniformLocation(lightShader->shaderProgram, "pointLights[0].specular"), 1.0f, 1.0f, 1.0f);
	glUniform1f(glGetUniformLocation(lightShader->shaderProgram, "pointLights[0].constant"), 1.0f);
	glUniform1f(glGetUniformLocation(lightShader->shaderProgram, "pointLights[0].linear"), 0.09f);
	glUniform1f(glGetUniformLocation(lightShader->shaderProgram, "pointLights[0].quadratic"), 0.032f);

	// Point light 2
	glUniform3f(glGetUniformLocation(lightShader->shaderProgram, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
	glUniform3f(glGetUniformLocation(lightShader->shaderProgram, "pointLights[1].ambient"), 0.05f, 0.05f, 0.05f);
	glUniform3f(glGetUniformLocation(lightShader->shaderProgram, "pointLights[1].diffuse"), pointLightColours[1].x, pointLightColours[1].y, pointLightColours[1].z);
	glUniform3f(glGetUniformLocation(lightShader->shaderProgram, "pointLights[1].specular"), 1.0f, 1.0f, 1.0f);
	glUniform1f(glGetUniformLocation(lightShader->shaderProgram, "pointLights[1].constant"), 1.0f);
	glUniform1f(glGetUniformLocation(lightShader->shaderProgram, "pointLights[1].linear"), 0.09f);
	glUniform1f(glGetUniformLocation(lightShader->shaderProgram, "pointLights[1].quadratic"), 0.032f);

	// Point light 3
	glUniform3f(glGetUniformLocation(lightShader->shaderProgram, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
	glUniform3f(glGetUniformLocation(lightShader->shaderProgram, "pointLights[2].ambient"), 0.05f, 0.05f, 0.05f);
	glUniform3f(glGetUniformLocation(lightShader->shaderProgram, "pointLights[2].diffuse"), pointLightColours[2].x, pointLightColours[2].y, pointLightColours[2].z);
	glUniform3f(glGetUniformLocation(lightShader->shaderProgram, "pointLights[2].specular"), 1.0f, 1.0f, 1.0f);
	glUniform1f(glGetUniformLocation(lightShader->shaderProgram, "pointLights[2].constant"), 1.0f);
	glUniform1f(glGetUniformLocation(lightShader->shaderProgram, "pointLights[2].linear"), 0.09f);
	glUniform1f(glGetUniformLocation(lightShader->shaderProgram, "pointLights[2].quadratic"), 0.032f);

	// Point light 4
	glUniform3f(glGetUniformLocation(lightShader->shaderProgram, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
	glUniform3f(glGetUniformLocation(lightShader->shaderProgram, "pointLights[3].ambient"), 0.05f, 0.05f, 0.05f);
	glUniform3f(glGetUniformLocation(lightShader->shaderProgram, "pointLights[3].diffuse"), pointLightColours[3].x, pointLightColours[3].y, pointLightColours[3].z);
	glUniform3f(glGetUniformLocation(lightShader->shaderProgram, "pointLights[3].specular"), 1.0f, 1.0f, 1.0f);
	glUniform1f(glGetUniformLocation(lightShader->shaderProgram, "pointLights[3].constant"), 1.0f);
	glUniform1f(glGetUniformLocation(lightShader->shaderProgram, "pointLights[3].linear"), 0.09f);
	glUniform1f(glGetUniformLocation(lightShader->shaderProgram, "pointLights[3].quadratic"), 0.032f);

	// SpotLight
	glUniform3f(glGetUniformLocation(lightShader->shaderProgram, "spotLight.position"), ourCamera.getPosition().x, ourCamera.getPosition().y, ourCamera.getPosition().z);
	glUniform3f(glGetUniformLocation(lightShader->shaderProgram, "spotLight.direction"), ourCamera.getFront().x, ourCamera.getFront().y, ourCamera.getFront().z);
	glUniform3f(glGetUniformLocation(lightShader->shaderProgram, "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
	glUniform3f(glGetUniformLocation(lightShader->shaderProgram, "spotLight.diffuse"), 1.0f, 1.0f, 1.0f);
	glUniform3f(glGetUniformLocation(lightShader->shaderProgram, "spotLight.specular"), 1.0f, 1.0f, 1.0f);
	glUniform1f(glGetUniformLocation(lightShader->shaderProgram, "spotLight.constant"), 1.0f);
	glUniform1f(glGetUniformLocation(lightShader->shaderProgram, "spotLight.linear"), 0.09f);
	glUniform1f(glGetUniformLocation(lightShader->shaderProgram, "spotLight.quadratic"), 0.032f);
	glUniform1f(glGetUniformLocation(lightShader->shaderProgram, "spotLight.cutOff"), glm::cos(glm::radians(7.5f)));
	glUniform1f(glGetUniformLocation(lightShader->shaderProgram, "spotLight.outerCutOff"), glm::cos(glm::radians(11.0f)));
}
