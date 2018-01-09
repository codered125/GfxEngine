#include <iostream>
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL2\src\SOIL2\SOIL2.h>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtx\matrix_decompose.hpp>
#include <gtc\type_ptr.hpp>
#include "Shader.h"
#include "Cube.h"
#include "Model.h"
#include "Camera.h"
#include "Light.h"
#include "TextureLoading.h"

//const GLint width = 1200, height = 800;
const GLint width = 1920, height = 1080;
void GLFWSetUp();
int SCREEN_WIDTH, SCREEN_HEIGHT;

void KeyCallback(GLFWwindow  * window, int key, int scancode, int action, int mode);
void ScrollCallback(GLFWwindow * window, double xOffset, double yOffset);
void MouseCallback(GLFWwindow * window, double xPos, double yPos);
void DoMovement();
void Tick();

void initialiseLights(Shader * lightShader);
void DrawLights(Shader * lampShader);
void DrawSkybox(Shader * skyboxShaderRef, GLuint * facesRef);
void DrawModel(Shader * modelShader, Model * ourModel, glm::mat4 model, bool depthTest);
void DrawBox(Shader * floorShader, glm::mat4 Transformation, GLuint * difftex, GLuint * spectex, bool depthTest, GLuint * acubeVbo, GLuint * acubeVAO);
void SetQuadUp(GLuint * quadVAO, GLuint * quadVBO);
void SetCubexVertUp(GLuint * cubeVAO, GLuint * cubeVBO);

Camera ourCamera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = width / 2.0f;
GLfloat lastY = height / 2.0f;
GLfloat deltaTime, SecondCounter, lastFrame = 0.0f;
Cube base;
bool Keys[1024];
bool firstMouse = true;
bool hdr = false;
int AliasingCount = 16;
glm::vec3 pointLightPositions[] =
{
	glm::vec3(2.0f, 0.0f, 3.0f), //yellow
	glm::vec3(2.0f, 0.0f, -2.0f), //Greeb
	glm::vec3(-2.0f, 0.0f, -2.0f), //Red
	glm::vec3(-2.0f, 0.0f, 3.0f) //Blue
};
glm::vec3 pointLightColours[] =
{

	glm::vec3(1.0f, 1.0f, 0.0f),//Yellow
	glm::vec3(0.0f, 1.0f, 0.0f),//Green
	glm::vec3(1.0f, 0.0f, 0.0f),//Red
	glm::vec3(0.0f, 0.0f, 1.0f)//Blue
};
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);


int main()
{

	GLFWSetUp();

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
	glewInit();
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
	glEnable(GL_MULTISAMPLE);

	Shader Modelshader("Shaders/modelLoading.vs", "Shaders/modelLoading.frag");
	Shader skyboxShader("Shaders/Skybox.vs", "Shaders/Skybox.frag");
	Shader lampShader("Shaders/Lamp.vs", "Shaders/Lamp.frag");
	Shader screenShader("Shaders/framebuffersScreen.vs", "Shaders/framebuffersScreen.frag");

	Model oldMan("Models/OldMan/muro.obj");
	Model Nanosuit("Models/Nanosuit/nanosuit.obj");
	Model deskModel("Models/Desk/Obj/Greydon Desk.obj");


	// Cubemap (Skybox)
	vector<const GLchar*> faces;
	faces.push_back("Images/HRSkybox/right.png");
	faces.push_back("Images/HRSkybox/left.png");
	faces.push_back("Images/HRSkybox/top.png");
	faces.push_back("Images/HRSkybox/bottom.png");
	faces.push_back("Images/HRSkybox/back.png");
	faces.push_back("Images/HRSkybox/front.png");
	GLuint cubemapTexture = TextureLoading::LoadCubemap(faces);
	GLuint floorTexture = TextureLoading::LoadTexture("Images/WoodPlanks_a_BaseColor.png");
	//GLuint floorTexture = TextureLoading::LoadTexture("Images/wood.jpg");
	GLuint floorTexSpec = TextureLoading::LoadTexture("Images/box_spec.png");


	GLuint cubeVBO, cubeVAO;
	SetCubexVertUp(&cubeVAO, &cubeVBO);

	GLuint quadVAO, quadVBO;
	SetQuadUp(&quadVAO, &quadVBO);


	GLuint fbo;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	GLuint texColourBuffer;
	glGenTextures(1, &texColourBuffer);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texColourBuffer);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, AliasingCount, GL_RGB, SCREEN_WIDTH, SCREEN_HEIGHT, GL_TRUE);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, texColourBuffer, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	GLuint rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, AliasingCount, GL_DEPTH24_STENCIL8, SCREEN_WIDTH, SCREEN_HEIGHT);

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColourBuffer, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// configure second post-processing framebuffer
	GLuint intermediateFBO;
	glGenFramebuffers(1, &intermediateFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, intermediateFBO);

	GLuint screenTexture;
	glGenTextures(1, &screenTexture);
	glBindTexture(GL_TEXTURE_2D, screenTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screenTexture, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) std::cout << "Failed  intermediatte Renderbuffer" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glm::mat4 FOV = glm::perspective(ourCamera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 1000.0f);

	screenShader.use();
	screenShader.setInt("screenTexture", 0);

	while (!glfwWindowShouldClose(window))
	{
		Tick();
		//check for events/inputs
		glfwPollEvents();
		DoMovement();

		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)							 
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);// make sure we clear the framebuffer's content
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//RENDER
		DrawSkybox(&skyboxShader, &cubemapTexture);
		DrawLights(&lampShader);

		glm::mat4 modelTransformation = glm::mat4();
		modelTransformation = glm::translate(modelTransformation, glm::vec3(0.0f, -1.75f, 0.0f));
		modelTransformation = glm::scale(modelTransformation, glm::vec3(0.01f, 0.01f, 0.01f));
		DrawModel(&Modelshader, &oldMan, modelTransformation, false);


		modelTransformation = glm::mat4();
		modelTransformation = glm::translate(modelTransformation, glm::vec3(1.0f, -1.75f, -1.50f));
		modelTransformation = glm::scale(modelTransformation, glm::vec3(0.125f, 0.125f, 0.125f));
		DrawModel(&Modelshader, &Nanosuit, modelTransformation, false);


		modelTransformation = glm::mat4();
		modelTransformation = glm::translate(modelTransformation, glm::vec3(0.0f, -1.75f, 3.75f));
		modelTransformation = glm::scale(modelTransformation, glm::vec3(0.01f, 0.01f, 0.01f));
		modelTransformation = glm::rotate(modelTransformation, glm::degrees(0.3f), glm::vec3(0.0f, 1.0f, 0.0f));
		DrawModel(&Modelshader, &deskModel, modelTransformation, false);


		modelTransformation = glm::mat4();

		modelTransformation = glm::scale(modelTransformation, glm::vec3(10.0f, 0.5f, 10.0f));
		modelTransformation = glm::translate(modelTransformation, glm::vec3(0.0f, -4.0f, 0.05));
		DrawBox(&Modelshader, modelTransformation, &floorTexture, &floorTexSpec, false, &cubeVBO, &cubeVAO);


		modelTransformation = glm::mat4();
	//	modelTransformation = glm::rotate(modelTransformation, 70.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		modelTransformation = glm::translate(modelTransformation, glm::vec3(0.0f, -4.0f, 0.05));
		modelTransformation = glm::scale(modelTransformation, glm::vec3(0.5f, 10.0f, 10.0f));
		DrawBox(&Modelshader, modelTransformation, &floorTexture, &floorTexSpec, false, &cubeVBO, &cubeVAO);



		//Blit multisampled buffer(s) to normal colorbuffer of intermediate FBO.Image is stored in screenTexture
		glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, intermediateFBO);
		glBlitFramebuffer(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GL_COLOR_BUFFER_BIT, GL_NEAREST);
		// now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		// clear all relevant buffers
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.

		screenShader.use();
		screenShader.setInt("screenTexture", 0);
		glBindVertexArray(quadVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, screenTexture);	// use the color attachment texture as the texture of the quad plane
		//hdr = true;
		screenShader.setBool("hdr", hdr);
		screenShader.setFloat("exposure", 0.50f);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		//swap screen buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return EXIT_SUCCESS;
}

void  GLFWSetUp()
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
	glfwWindowHint(GLFW_SAMPLES, AliasingCount);
	glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
	glEnable(GL_MULTISAMPLE);
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
	//SecondCounter = 1;
	SecondCounter = SecondCounter >= 1 ? 0 : SecondCounter + (deltaTime / 12);

}

void DrawLights(Shader * lampShader)
{
	lampShader->use();
	GLuint lightVAO, lightVBO;

	//glGenBuffers(1, &lightVBO);
	//glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	glEnableVertexAttribArray(0);
	glBufferData(GL_ARRAY_BUFFER, sizeof(base.vertices), base.vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);

	glm::mat4 model;
	glm::mat4 FOV = glm::perspective(ourCamera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 1000.0f);
	lampShader->setMat4("projection", FOV);
	glm::mat4 view = ourCamera.GetViewMatrix();
	lampShader->setMat4("view", view);
	const float min = 0, max = 1;
	float rotAngle = min + ((max - min) *SecondCounter);

	glBindVertexArray(lightVAO);
	for (GLuint i = 0; i < 4; i++)
	{
		model = glm::mat4();
		model = glm::translate(model, pointLightPositions[i]);
		model = glm::scale(model, glm::vec3(0.2f));
		//model = glm::rotate(model, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lampShader->shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(glGetUniformLocation(lampShader->shaderProgram, "inColour"), 1, &pointLightColours[i][0]);
		lampShader->setFloat("Time", SecondCounter);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	glBindVertexArray(0);
}

void DrawSkybox(Shader * skyboxShaderRef, GLuint *facesRef)
{
	// Draw skybox as last
	// skybox cube
	skyboxShaderRef->use();

	// Setup skybox VAO
	GLuint skyboxVAO, skyboxVBO;

	glGenBuffers(1, &skyboxVBO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glGenVertexArrays(1, &skyboxVAO);
	glBindVertexArray(skyboxVAO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(base.skyboxVertices), &base.skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);


	glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content
	glm::mat4 view = glm::mat4(glm::mat3(ourCamera.GetViewMatrix()));	// Remove any translation component of the view matrix
	glUniformMatrix4fv(glGetUniformLocation(skyboxShaderRef->shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));

	glm::mat4 FOV;
	FOV = glm::perspective(ourCamera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);
	glUniformMatrix4fv(glGetUniformLocation(skyboxShaderRef->shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(FOV));

	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	//GLuint cubemapTexture = TextureLoading::LoadCubemap(*facesRef);
	glBindTexture(GL_TEXTURE_CUBE_MAP, *facesRef);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS); // Set depth function back to default


}

void DrawModel(Shader * modelShader, Model * ourModel, glm::mat4 model, bool depthTest)
{
	modelShader->use();
	modelShader->setFloat("Time", SecondCounter);

	glm::mat4 FOV;
	FOV = glm::perspective(ourCamera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 1000.0f);

	glm::mat4 view = ourCamera.GetViewMatrix();
	glUniformMatrix4fv(glGetUniformLocation(modelShader->shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(FOV));
	glUniformMatrix4fv(glGetUniformLocation(modelShader->shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(modelShader->shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	initialiseLights(modelShader);
	ourModel->Draw(modelShader);
}

void DrawBox(Shader * floorShader, glm::mat4 Transformation, GLuint * difftex, GLuint * spectex, bool depthTest, GLuint * acubeVbo, GLuint * acubeVAO)
{
	floorShader->use();
	glUniform1i(glGetUniformLocation(floorShader->shaderProgram, "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(floorShader->shaderProgram, "material.specular"), 1);
	floorShader->setFloat("material.shininess", 16.0f);
	floorShader->setFloat("Time", SecondCounter);
	glm::mat4 FOV = glm::perspective(ourCamera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 1000.0f);
	floorShader->setMat4("projection", FOV);
	glUniformMatrix4fv(glGetUniformLocation(floorShader->shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(FOV));

	glm::mat4 view = ourCamera.GetViewMatrix();
	floorShader->setMat4("view", view);
	glUniformMatrix4fv(glGetUniformLocation(floorShader->shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));

	glBindVertexArray(*acubeVAO);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, *difftex);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, *spectex);
	glUniformMatrix4fv(glGetUniformLocation(floorShader->shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(Transformation));
	initialiseLights(floorShader);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void SetQuadUp(GLuint * quadVAO, GLuint * quadVBO)
{
	glGenVertexArrays(1, quadVAO);
	glGenBuffers(1, quadVBO);
	glBindVertexArray(*quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, *quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(base.quadVertices), &base.quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}

void SetCubexVertUp(GLuint * cubeVAO, GLuint * cubeVBO)
{
	glGenBuffers(1, cubeVBO);
	glBindBuffer(GL_ARRAY_BUFFER, *cubeVBO);

	glGenVertexArrays(1, cubeVAO);
	glBindVertexArray(*cubeVAO);

	//vert data
	glBufferData(GL_ARRAY_BUFFER, sizeof(base.vertices), &base.vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//normal data
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	//tex coord data
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (GLvoid *)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
}

void initialiseLights(Shader * lightShader)
{

	// Here we set all the uniforms for the 5/6 types of lights we have. We have to set them manually and index
	// the proper PointLight struct in the array to set each uniform variable. This can be done more code-friendly
	// by defining light types as classes and set their values in there, or by using a more efficient uniform approach
	// by using 'Uniform buffer objects', but that is something we discuss in the 'Advanced GLSL' tutorial.


	// Directional light
	Light Directional0 = DirectionalLight(lightShader, "dirLight");
	Directional0.direction = glm::vec3(-0.2f, -1.0f, -0.3f);
	Directional0.ambient = glm::vec3(0.05f);
	Directional0.diffuse = glm::vec3(0.4f);
	Directional0.specular = glm::vec3(0.5f, 0.5f, 0.2f);
	Directional0.setUpShader();


	// Point light 1
	Light Point0 = PointLight(lightShader, "pointLights[0]");
	Point0.diffuse = glm::vec3(pointLightColours[0].x, pointLightColours[0].y, pointLightColours[0].z);
	Point0.position = glm::vec3(pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
	Point0.specular = Point0.diffuse;
	Point0.setUpShader();


	// Point light 2
	Light Point1 = PointLight(lightShader, "pointLights[1]");
	Point1.diffuse = glm::vec3(pointLightColours[1].x, pointLightColours[1].y, pointLightColours[1].z);
	Point1.position = glm::vec3(pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
	Point1.specular = Point1.diffuse;
	Point1.setUpShader();

	// Point light 3
	Light Point2 = PointLight(lightShader, "pointLights[2]");
	Point2.diffuse = glm::vec3(pointLightColours[2].x, pointLightColours[2].y, pointLightColours[2].z);
	Point2.position = glm::vec3(pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
	Point2.specular = Point2.diffuse;
	Point2.setUpShader();

	// Point light 4
	Light Point3 = PointLight(lightShader, "pointLights[3]");
	Point3.diffuse = glm::vec3(pointLightColours[3].x, pointLightColours[3].y, pointLightColours[3].z);
	Point3.position = glm::vec3(pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
	Point3.specular = Point3.diffuse;
	Point3.setUpShader();


	// SpotLight
	Light CameraSpot = SpotLight(lightShader, "spotLight");
	CameraSpot.position = ourCamera.getPosition();
	CameraSpot.direction = ourCamera.getFront();
	CameraSpot.ambient = glm::vec3(0.0f);
	CameraSpot.diffuse = glm::vec3(1.0f);
	CameraSpot.specular = glm::vec3(1.0f);
	CameraSpot.setUpShader();



}
