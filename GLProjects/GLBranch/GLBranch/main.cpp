#include "main.h"

#include <iostream>
#include <thread>
#include <future>

//-------------------------------------------------------------------------------------

#include <SOIL2\src\SOIL2\SOIL2.h>

//-------------------------------------------------------------------------------------
#include "Source/Public/GlfwInterface.h"
#include "Source/Public/Shader.h"
#include "Source/Public/StaticVertices.h"
#include "Source/Public/Model.h"
#include "Source/Public/Inputs.h"
#include "Source/Public/Camera.h"
#include "Source/Public/PostProcessing.h"
#include "Source/Public/Light.h"
#include "Source/Public/TextureLoading.h"
#include "Source/Public/Math.h"

//-------------------------------------------------------------------------------------

int main()
{
	currentPostProcessSettings = &PostProcessSettings();
	currentPostProcessSettings->HDR = EffectStatus::Active;
	ourCamera = &Camera(glm::vec3(0.0f, 1.0f, 3.0f));

	LightingCamera = &Camera(StaticVertices::pointLightPositions[0], glm::vec3(0.0, 1.0, 1.0), true, 1024.0f / 1024.0f, 1.0f, 7.5f);

	auto* window = GlfwInterface::DefineAndCreaateWindow(AliasingCount, height, width);
	if (window == nullptr) 
	{
		return EXIT_FAILURE;
	};
	GlfwInterface::GetFramebufferSize(window, SCREEN_WIDTH, SCREEN_HEIGHT);
	GlfwInterface::SetInputCallbackFunctions(window, KeyCallback, ScrollCallback, MouseCallback);

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


	auto UnlitShader = Shader("Shaders/Unlit.vs", "Shaders/Unlit.frag");
	auto PBRshader = Shader("Shaders/IBLPBR.vs", "Shaders/IBLPBR.frag");
	auto BlinnPhongShader = Shader("Shaders/BlinnPhong.vs", "Shaders/BlinnPhong.frag");
	auto skyboxShader = Shader("Shaders/Skybox.vs", "Shaders/Skybox.frag");
	auto lampShader = Shader("Shaders/Lamp.vs", "Shaders/Lamp.frag");
	auto DepthShader = Shader("Shaders/ShadowMapping.vs", "Shaders/ShadowMapping.frag");
	auto screenShader = Shader("Shaders/framebuffersScreen.vs", "Shaders/framebuffersScreen.frag");

	auto roomModel = Model("Models/Sponza/Sponza.obj");
	auto GizMo = Model("Models/Gizmo/GizmoForMo.obj");

	// Cubemap (Skybox)
	std::vector<const GLchar*> SkyboxFaces;
	//Right, left, top, bottom, back, front
	SkyboxFaces.push_back("Images/WaveEngineSkybox/posx.bmp");
	SkyboxFaces.push_back("Images/WaveEngineSkybox/negx.bmp");
	SkyboxFaces.push_back("Images/WaveEngineSkybox/posy.bmp");
	SkyboxFaces.push_back("Images/WaveEngineSkybox/negy.bmp");
	SkyboxFaces.push_back("Images/WaveEngineSkybox/negz.bmp");
	SkyboxFaces.push_back("Images/WaveEngineSkybox/posz.bmp");
	auto SkyboxTexture = TextureLoading::LoadCubemap(SkyboxFaces);

	auto IBLTexture = TextureLoading::LoadCubemap(SkyboxFaces);

	// Cubemap (Skybox)
	std::vector<const GLchar*> IBLFaces;


	GLuint quadVAO, quadVBO;
	SetQuadUp(&quadVAO, &quadVBO);

	//Creating our framebuffer pointer
	GLuint fbo;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	//Create a colour buffer 
	GLuint texColourBuffer[2];
	glGenTextures(2, texColourBuffer);
	for (GLuint i = 0; i < 2; i++)
	{
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texColourBuffer[i]);
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, AliasingCount, GL_RGBA16F, SCREEN_WIDTH, SCREEN_HEIGHT, GL_TRUE);
		glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// we clamp to the edge as the blur filter would otherwise sample repeated texture values!
		glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D_MULTISAMPLE, texColourBuffer[i], 0);
	}

	GLuint attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	//Creating our render buffer object (RBO are write-only)
	//possible type of framebuffer attachments,
	//These things are quite to copy information from and put
	//stores its data in OpenGL's native rendering format making it optimized for off-screen rendering to a framebuffer
	GLuint rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, AliasingCount, GL_DEPTH24_STENCIL8, SCREEN_WIDTH, SCREEN_HEIGHT);

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColourBuffer[0], 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//configure second post-processing framebuffer
	//Pushes our texture into our 
	GLuint intermediateFBO;
	glGenFramebuffers(1, &intermediateFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, intermediateFBO);

	GLuint screenTexture;
	glGenTextures(1, &screenTexture);
	glBindTexture(GL_TEXTURE_2D, screenTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screenTexture, 0);
	

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Failed  intermediatte Renderbuffer" << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//Frame buffer object for rendering depth map
	GLuint DepthMapFBO;
	glGenFramebuffers(1, &DepthMapFBO);

	//2D Texture for FBO depth buffer
	const GLuint ShadowWidth = 1024,  ShadowHeight = 1024;
	GLuint DepthMap;
	glGenTextures(1, &DepthMap);
	glBindTexture(GL_TEXTURE_2D, DepthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, ShadowWidth, ShadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindFramebuffer(GL_FRAMEBUFFER, DepthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, DepthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	DepthShader.use();
	DepthShader.setInt("ShadowMap", DepthMap);

//	glm::mat4 FOV = glm::perspective(Camera::GetZoom(ourCamera), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 1000.0f);

	screenShader.use();
	screenShader.setInt("screenTexture", 0);
	Input::InitializeInputMap(InputMap);

	while (!GlfwInterface::WindowShouldClose(window))
	{
		//TICK
		Tick();
		//check for events/inputs
		glfwPollEvents();

		//INPUTS
		Input::DoMovement(deltaTime, ourCamera, Keys, keyboardlockout, currentPostProcessSettings, InputMap);

		//Shadow Render Pass
		glViewport(0, 0, ShadowWidth, ShadowHeight);
		glBindFramebuffer(GL_FRAMEBUFFER, DepthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		RenderDemo(&lampShader, &skyboxShader, &SkyboxTexture, &DepthShader, &roomModel, &UnlitShader, &GizMo, &IBLTexture, LightingCamera, &DepthMap);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		
		//Normal Render Pass
		glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)							 
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);// make sure we clear the framebuffer's content
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		auto AspectRatio = Camera::GetAspectRatio(ourCamera);
		RenderDemo(&lampShader, &skyboxShader, &SkyboxTexture, &BlinnPhongShader, &roomModel, &UnlitShader, &GizMo, &IBLTexture, ourCamera, &DepthMap);

		//Multisampled image contains more informmation than normal images, blits downscales / resolves the image
		//Copies a region from one framebuffer ( our read buffer) to another buffer(our draw buffer)
		glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, intermediateFBO);
   		glBlitFramebuffer(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GL_COLOR_BUFFER_BIT, GL_NEAREST);

		
		//PostProcess Render Pass
		// clear all relevant buffers
		glBindFramebuffer(GL_FRAMEBUFFER, 0);// now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.

		currentPostProcessSettings->HDR = EffectStatus::Active;
		PostProcessing::ApplyEffects(&screenShader, currentPostProcessSettings);

		glBindVertexArray(quadVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, screenTexture);	// use the color attachment texture as the texture of the quad plane
		//glActiveTexture(GL_TEXTURE1);
		//glBindTexture(GL_TEXTURE_2D, screenTexture[1]);

		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
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
	//Calculate framedata
	const auto currentFrame = (GLfloat)glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	
	SecondCounter = MoMath::MoSaturate(SecondCounter);

	lightDirection = (SecondCounter >= 1 || SecondCounter <= 0.0) ? !lightDirection : lightDirection;
	keyboardlockout = keyboardlockout > 0 ? keyboardlockout - deltaTime : 0;

	if (currentPostProcessSettings->TimeBasedEffects == EffectStatus::Active)
	{
		const auto currentDelt = lightDirection ? deltaTime : deltaTime * -1;
		SecondCounter += (currentDelt / 6);
	}
}

//-------------------------------------------------------------------

void DrawLights(Shader * lampShader, Camera* Perspective)
{
	lampShader->use();
	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	glEnableVertexAttribArray(0);
	glBufferData(GL_ARRAY_BUFFER, sizeof(StaticVertices::vertices), StaticVertices::vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);

	glm::mat4 model;
	glm::mat4 FOV = Camera::GetProjection(Perspective);
	lampShader->setMat4("projection", FOV);
	glm::mat4 view = Camera::GetViewMatrix(Perspective);
	lampShader->setMat4("view", view);

	glBindVertexArray(lightVAO);
	//for (GLuint i = 0; i < pointLightPositions->length(); i++)
	for (GLuint i = 0; i < 3; i++)
	{
		model = glm::mat4();
		model = glm::translate(model, StaticVertices::pointLightPositions[i]); //pointLightPositions[i]);
		model = glm::scale(model, glm::vec3(0.2f));
		glUniformMatrix4fv(glGetUniformLocation(lampShader->shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(glGetUniformLocation(lampShader->shaderProgram, "inColour"), 1, &StaticVertices::pointLightColours[i][0]);
		lampShader->setFloat("Time", SecondCounter);
		lampShader->setFloat("TimeLapsed", (float)glfwGetTime());
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	glBindVertexArray(0);
}

//-------------------------------------------------------------------

void DrawSkybox(Shader * skyboxShaderRef, GLuint * facesRef, Camera* Perspective)
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(StaticVertices::skyboxVertices), &StaticVertices::skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);


	glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content
	glm::mat4 view = glm::mat4(glm::mat3(Camera::GetViewMatrix(Perspective)));	// Remove any translation component of the view matrix
	glUniformMatrix4fv(glGetUniformLocation(skyboxShaderRef->shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));

	glm::mat4 FOV;
	FOV = Camera::GetProjection(Perspective);
	glUniformMatrix4fv(glGetUniformLocation(skyboxShaderRef->shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(FOV));

	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	//GLuint cubemapTexture = TextureLoading::LoadCubemap(*facesRef);
	glBindTexture(GL_TEXTURE_CUBE_MAP, *facesRef);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS); // Set depth function back to default
}

//-------------------------------------------------------------------

void DrawModel(Shader * modelShader, Model * ourModel, glm::mat4 model, GLuint *facesRef, Camera* Perspective, GLuint * ShadowMap, float shine)
{
	modelShader->use();
	modelShader->setFloat("Time", SecondCounter);
	modelShader->setFloat("TimeLapsed", (float)glfwGetTime());
	modelShader->setFloat("NearPlane", Camera::GetNearPlane(Perspective));
	modelShader->setFloat("FarPlane", Camera::GetFarPlane(Perspective));
	modelShader->setVec3("CamPos", Camera::getPosition(Perspective));
	modelShader->setVec3("CamDir", Camera::getFront(Perspective));
	modelShader->SetSampler("ShadowMap", ShadowMap, GL_TEXTURE_2D);
	
	glm::mat4 FOV = Camera::GetProjection(Perspective);//glm::perspective(Camera::GetZoom(Perspective), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 1000.0f);
	glm::mat4 view = Camera::GetViewMatrix(Perspective);
	
	glUniformMatrix4fv(glGetUniformLocation(modelShader->shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(FOV));
	glUniformMatrix4fv(glGetUniformLocation(modelShader->shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(modelShader->shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(modelShader->shaderProgram, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(FOV * view ));
	glBindTexture(GL_TEXTURE_CUBE_MAP, *facesRef);
	initialiseLights(modelShader);
	ourModel->Draw(modelShader, shine);
}


//-------------------------------------------------------------------

void DrawBox(Shader * floorShader, glm::mat4 Transformation, GLuint * difftex, GLuint * spectex, bool depthTest, GLuint * acubeVbo, GLuint * acubeVAO)
{
	floorShader->use();
	glUniform1i(glGetUniformLocation(floorShader->shaderProgram, "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(floorShader->shaderProgram, "material.specular"), 1);

	floorShader->setFloat("material.shininess", 16.0f);
	floorShader->setFloat("Time", SecondCounter);
	glm::mat4 FOV = glm::perspective(Camera::GetZoom(ourCamera), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 1000.0f);
	floorShader->setMat4("projection", FOV);

	glm::mat4 view = Camera::GetViewMatrix(ourCamera);
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

//-------------------------------------------------------------------

void SetQuadUp(GLuint * quadVAO, GLuint * quadVBO)
{
	glGenVertexArrays(1, quadVAO);
	glGenBuffers(1, quadVBO);
	glBindVertexArray(*quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, *quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(StaticVertices::quadVertices), &StaticVertices::quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}

//-------------------------------------------------------------------

void initialiseLights(Shader * lightShader)
{
	// Directional light
	auto Directional0 = Light::DirectionalLight(lightShader, "dirLight");
	Directional0.direction = glm::vec3(0.0, 1.0, 1.0);
	Directional0.ambient = MoMath::MoNormalize(glm::vec3(0.05f));
	Directional0.diffuse = MoMath::MoNormalize(glm::vec3(1, 1, 1));
	Directional0.specular = MoMath::MoNormalize(glm::vec3(1, 1, 1));
	Directional0.intensity = 0;
	Directional0.setUpShader();

	
	// Point light 1
	auto Point0 = Light::PointLight(lightShader, "pointLights[0]");
	Point0.diffuse = MoMath::MoNormalize(StaticVertices::pointLightColours[0]);
	Point0.position = StaticVertices::pointLightPositions[0]; // MyLerp(pointLightPositions[1], pointLightPositions[0], SecondCounter);
	Point0.ambient = glm::vec3(0.0f);
	Point0.specular = glm::vec3(0.0f);
	Point0.intensity = 200;
	Point0.setUpShader();

	/*
	// Point light 1
	Light Point1 = Light::PointLight(lightShader, "pointLights[1]");
	Point1.diffuse = StaticVertices::pointLightColours[1];
	Point1.position = StaticVertices::pointLightPositions[1];//MyLerp(pointLightPositions[2], pointLightPositions[1], SecondCounter);//glm::vec3(pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
	Point1.ambient = glm::vec3(0.0f);
	Point1.specular = glm::vec3(0.0f);
	Point1.intensity = 10.0f; //* SecondCounter;
	Point1.setUpShader();

	// Point light 1
	Light Point2 = Light::PointLight(lightShader, "pointLights[2]");
	Point2.diffuse = StaticVertices::pointLightColours[2];
	Point2.position = StaticVertices::pointLightPositions[2];// MyLerp(pointLightPositions[0], pointLightPositions[1], SecondCounter); //glm::vec3(pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
	Point2.ambient = glm::vec3(0.0f);
	Point2.specular = glm::vec3(0.0f);
	Point2.intensity = 7.5f;//* SecondCounter;
	Point2.setUpShader();
	*/


	// SpotLight
	auto CameraSpot = Light::SpotLight(lightShader, "spotLight");
	CameraSpot.position = Camera::getPosition(ourCamera);
	CameraSpot.direction = Camera::getFront(ourCamera);
	CameraSpot.ambient = glm::vec3(0.0f);
	CameraSpot.diffuse = glm::vec3(1.0f);
	CameraSpot.specular = glm::vec3(1.0f);
	CameraSpot.setUpShader();
}

//-------------------------------------------------------------------

void KeyCallback(GLFWwindow * window, int key, int scancode, int action, int mode)
{
	Input::KeyCallback(window, key, scancode, action, mode, ourCamera, Keys);
}

//-------------------------------------------------------------------

void ScrollCallback(GLFWwindow * window, double xOffset, double yOffset)
{
	Input::ScrollCallback(window, xOffset, yOffset, ourCamera);
}

//-------------------------------------------------------------------

void MouseCallback(GLFWwindow * window, double xPos, double yPos)
{
	Input::MouseCallback(window, xPos, yPos, ourCamera, lastX, lastY, firstMouse);
}

void RenderDemo(Shader* InLampShader, Shader* InSkyboxShader, GLuint* InSkyboxTexture, Shader* InModelShader, Model* InModel, Shader* InUnlitShader, Model* InGizmo, GLuint* IBLTexture, Camera* Perspective, GLuint * ShadowMap)
{
	//RENDER
	DrawSkybox(InSkyboxShader, InSkyboxTexture, Perspective);
	DrawLights(InLampShader, Perspective);

	auto modelTransformation = glm::mat4();

	//Room Model
	modelTransformation = glm::mat4();
	modelTransformation = glm::scale(modelTransformation, glm::vec3(0.01f));
	DrawModel(InModelShader, InModel, modelTransformation, IBLTexture, Perspective, ShadowMap, 1.0f);


	//modelTransformation = glm::mat4();
	//modelTransformation = glm::scale(modelTransformation, glm::vec3(0.25f));
	//DrawModel(InUnlitShader, InGizmo, modelTransformation, IBLTexture, Perspective, ShadowMap, 1.0f);

	//modelTransformation = glm::scale(modelTransformation, glm::vec3(0.05f));
	//modelTransformation = glm::rotate(modelTransformation, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//modelTransformation = glm::translate(modelTransformation, glm::vec3(0.0f, -0.75f, 4.0f));
	//DrawWater(&WaterShader, &waterModel, modelTransformation);
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------