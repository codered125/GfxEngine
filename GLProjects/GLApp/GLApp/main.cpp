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
#include "Source/Public/SceneRenderTarget.h"

//-------------------------------------------------------------------------------------

#define DEBUGSHADOWMAP 0
int main()
{
	//Camera's and post process
	currentPostProcessSettings = &PostProcessSettings();
	currentPostProcessSettings->HDR = EffectStatus::Active;
	ourCamera = &Camera(glm::vec3(0.0f, 10.0f, 0.0f));
	LightingCamera = &Camera(StaticVertices::SunPos, StaticVertices::SunDir, true, 4096 / 4096, 0.10f, 50.5f);

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
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_DEPTH_TEST);
	std::ifstream IncludedFile;
	auto string = Shader::ParseShaderForIncludes("Shaders/TestIncludeSupport0.glsl");

	//Shaders & Models
	auto UnlitShader = Shader("Shaders/Unlit.vs", "Shaders/Unlit.frag");
	auto PBRshader = Shader("Shaders/IBLPBR.vs", "Shaders/IBLPBR.frag");
	auto skyboxShader = Shader("Shaders/Skybox.vs", "Shaders/Skybox.frag");
	auto lampShader = Shader("Shaders/Lamp.vs", "Shaders/Lamp.frag");
	auto DepthShader = Shader("Shaders/ShadowMapping.vs", "Shaders/ShadowMapping.frag");
	auto screenShader = Shader("Shaders/framebuffersScreen.vs", "Shaders/framebuffersScreen.frag");
	auto roomModel = Model("Models/SponzaTest/sponza.obj");
	//auto roomModel = Model("Models/Room/Room.obj");
	auto GizMo = Model("Models/Gizmo/GizmoForMo.obj");
	auto ArrowLight = Model("Models/ArrowLight/ArrowLight.obj");

	std::vector<const GLchar*> SkyboxFaces;	// Cubemap (Skybox)
	//Right, left, top, bottom, back, front
	SkyboxFaces.push_back("Images/WaveEngineSkybox/posx.bmp");
	SkyboxFaces.push_back("Images/WaveEngineSkybox/negx.bmp");
	SkyboxFaces.push_back("Images/WaveEngineSkybox/posy.bmp");
	SkyboxFaces.push_back("Images/WaveEngineSkybox/negy.bmp");
	SkyboxFaces.push_back("Images/WaveEngineSkybox/negz.bmp");
	SkyboxFaces.push_back("Images/WaveEngineSkybox/posz.bmp");
	auto SkyboxTexture = TextureLoading::LoadCubemap(SkyboxFaces);

	GLuint quadVAO, quadVBO;
	SetQuadUp(&quadVAO, &quadVBO);


	SceneRenderTarget MainRenderTarget(SCREEN_WIDTH, SCREEN_HEIGHT, GL_TEXTURE_2D_MULTISAMPLE, GL_RGBA16F, GL_UNSIGNED_BYTE, 2, false, true);
	//(GLuint InWidth, GLuint InHeight, GLenum InTargetType, GLenum InInternalFormat, GLenum InFormat, GLuint InNrColourAttachments = 1, bool InMakeDepth = false, bool InMSAA = false)

	GLuint rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, AliasingCount, GL_DEPTH24_STENCIL8, SCREEN_WIDTH, SCREEN_HEIGHT);

	glBindFramebuffer(GL_FRAMEBUFFER, MainRenderTarget.GetID());
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	std::cout << "Main Time for  intermediatte " << std::endl;

	//configure second post-processing framebuffer
	//Pushes our texture into our 

	SceneRenderTarget IntermediateRenderTarget(SCREEN_WIDTH, SCREEN_HEIGHT, GL_TEXTURE_2D, GL_RGB16F, GL_RGBA, 1, false, false);
	SceneRenderTarget DepthRenderTarget(4096, 4096, GL_TEXTURE_2D, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, 0, true, false);


	Input::InitializeInputMap(InputMap);

	while (!GlfwInterface::WindowShouldClose(window))
	{
		//check for events/inputs
		glfwPollEvents();

		//INPUTS
		Tick();
		Input::DoMovement(deltaTime, ourCamera, Keys, keyboardlockout, currentPostProcessSettings, InputMap);

		//Shadow Render Pass
		glViewport(0, 0, std::get<0>(DepthRenderTarget.GetDepthTexture()->GetWidthAndHeightOfTexture()), std::get<1>(DepthRenderTarget.GetDepthTexture()->GetWidthAndHeightOfTexture()));
		glBindFramebuffer(GL_FRAMEBUFFER, DepthRenderTarget.GetID());
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)
		glCullFace(GL_FRONT);

		DepthShader.use();
		RenderDemo(nullptr, nullptr, &SkyboxTexture, &DepthShader, &roomModel, nullptr, &GizMo, LightingCamera, nullptr);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);


		glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, MainRenderTarget.GetID());

		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);// make sure we clear the framebuffer's content
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)	
		glCullFace(GL_BACK);
		RenderDemo(&lampShader, &skyboxShader, &SkyboxTexture, &PBRshader, &roomModel, &UnlitShader, &GizMo, ourCamera, &DepthRenderTarget.GetDepthTexture()->GetID());
		
		//Multisampled image contains more informmation than normal images, blits downscales / resolves the image
		//Copies a region from one framebuffer ( our read buffer) to another buffer(our draw buffer)
		glBindFramebuffer(GL_READ_FRAMEBUFFER, MainRenderTarget.GetID());
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, IntermediateRenderTarget.GetID());
   		glBlitFramebuffer(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GL_COLOR_BUFFER_BIT, GL_NEAREST);
		
		//PostProcess Render Pass
		// clear all relevant buffers
		glBindFramebuffer(GL_FRAMEBUFFER, 0);// now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
		glClearColor(1.0f, 0.0f, 1.0f, 1.0f); // set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
 
		glBindVertexArray(quadVAO);
		currentPostProcessSettings->HDR = EffectStatus::Active;
		PostProcessing::ApplyEffects(&screenShader, currentPostProcessSettings);
		screenShader.SetSampler("screenTexture", &IntermediateRenderTarget.GetColourAttachmentByIndex(0)->GetID(), GL_TEXTURE_2D);
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, IntermediateRenderTarget.GetColourAttachmentByIndex(0)->GetID());
		if (DEBUGSHADOWMAP)
		{
			glBindTexture(GL_TEXTURE_2D, DepthRenderTarget.GetDepthTexture()->GetID());	// use the color attachment texture as the texture of the quad plane
		}
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

void DrawLights(Shader * lampShader, Camera* Perspective, Model* InModel)
{
	lampShader->use();
	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	glEnableVertexAttribArray(0);
	glBufferData(GL_ARRAY_BUFFER, sizeof(StaticVertices::vertices), StaticVertices::vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);

	glm::mat4 model;
	glm::mat4 FOV = Camera::GetProjection(Perspective);
	lampShader->setMat4("projection", FOV);
	glm::mat4 view = Camera::GetViewMatrix(Perspective);
	lampShader->setMat4("view", view);


	model = glm::mat4();
	model = glm::translate(model, StaticVertices::SunPos); //pointLightPositions[i]);
	model = glm::scale(model, glm::vec3(2.f));
	glUniformMatrix4fv(glGetUniformLocation(lampShader->shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniform3fv(glGetUniformLocation(lampShader->shaderProgram, "inColour"), 1, &glm::vec3(50, 50, 50)[0]);
	lampShader->setFloat("Time", SecondCounter);
	lampShader->setFloat("TimeLapsed", (float)glfwGetTime());
	glDrawArrays(GL_TRIANGLES, 0, 36);

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

void DrawSkybox(Shader* skyboxShaderRef, GLuint* facesRef, Camera* Perspective)
{
	if (!skyboxShaderRef)
	{
		return;
	}

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
	skyboxShaderRef->setMat4("view", view);

	auto FOV = Camera::GetProjection(Perspective);
	skyboxShaderRef->setMat4("projection", FOV);

	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, *facesRef);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS); // Set depth function back to default
}

//-------------------------------------------------------------------

void DrawModel(Shader* modelShader, Model* ourModel, glm::mat4 model, Camera* Perspective, GLuint* ShadowMap)
{
	modelShader->use();
	if (ShadowMap)
	{
		glEnable(GL_TEXTURE_2D);
		auto i = modelShader->GetUniformLocation("ShadowMap");
		if (i != -1)
		{
			modelShader->setInt("ShadowMap", i);
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, *ShadowMap);
		}
	}

	modelShader->setFloat("NearPlane", Camera::GetNearPlane(Perspective));
	modelShader->setFloat("FarPlane", Camera::GetFarPlane(Perspective));
	modelShader->setVec3("CamPos", Camera::getPosition(Perspective));
	modelShader->setVec3("CamDir", Camera::getFront(Perspective));

	glm::mat4 FOV = Camera::GetProjection(Perspective);//glm::perspective(Camera::GetZoom(Perspective), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 1000.0f);
	glm::mat4 view = Camera::GetViewMatrix(Perspective);

	modelShader->setMat4("projection", FOV);
	modelShader->setMat4("view", view);
	initialiseLights(modelShader);

	glm::mat4 LightingFOV = Camera::GetProjection(LightingCamera);
	glm::mat4 LightingView = Camera::GetViewMatrix(LightingCamera);
	modelShader->setMat4("lightSpaceMatrix", LightingFOV * LightingView);
	modelShader->setMat4("model", model);
	ourModel->Draw(modelShader);
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

void Tick()
{
	const auto currentFrame = (GLfloat)glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	keyboardlockout = keyboardlockout > 0 ? keyboardlockout - deltaTime : 0;
}

//-------------------------------------------------------------------------------------

void initialiseLights(Shader * lightShader)
{

	const float pointIntes = 0.02f; const float directIntes = 0.05f;
	// Directional light
	auto Directional0 = Light::DirectionalLight(lightShader, "dirLight");
	Directional0.direction = StaticVertices::SunDir;//Camera::getFront(LightingCamera);
	Directional0.ambient = glm::vec3(1);
	Directional0.diffuse = glm::vec3(50, 50, 50);
	Directional0.specular = glm::vec3(1);
	Directional0.position = StaticVertices::SunPos;
	Directional0.intensity = directIntes;
	Directional0.setUpShader();
	
	// Point light 1
	for (int i = 0; StaticVertices::pointLightColours->length() > i; i++)
	{
		auto Point = Light::PointLight(lightShader, "pointLights[" + std::to_string(i) + "]");
		Point.diffuse = StaticVertices::pointLightColours[i];
		Point.position = StaticVertices::pointLightPositions[i]; // MyLerp(pointLightPositions[1], pointLightPositions[0], SecondCounter);
		Point.ambient = glm::vec3(1.0f);
		Point.specular = glm::vec3(1.0f);
		Point.intensity = pointIntes;
		Point.setUpShader();
	}
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

//-------------------------------------------------------------------------------------

void RenderDemo(Shader* InLampShader, Shader* InSkyboxShader, GLuint* InSkyboxTexture, Shader* InModelShader, Model* InModel, Shader* InUnlitShader, Model* InGizmo, Camera* Perspective, GLuint * ShadowMap)
{
	//Room Model
	auto modelTransformation = glm::mat4();
	modelTransformation = glm::scale(modelTransformation, glm::vec3(0.01f));
	//modelTransformation = glm::scale(modelTransformation, glm::vec3(0.5f));
	modelTransformation = glm::rotate(modelTransformation, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	DrawModel(InModelShader, InModel, modelTransformation, Perspective, ShadowMap);

	if (InUnlitShader)
	{
		modelTransformation = glm::mat4();
		modelTransformation = glm::scale(modelTransformation, glm::vec3(0.25f));
		modelTransformation = glm::translate(modelTransformation, glm::vec3(0.0f, 1.50f, 0.0f));
		DrawModel(InUnlitShader, InGizmo, modelTransformation, Perspective, ShadowMap);
	}

//RENDER 
//skip this for depth pass
	if (ShadowMap)
	{
		DrawSkybox(InSkyboxShader, InSkyboxTexture, Perspective);
		DrawLights(InLampShader, ourCamera, nullptr);
	}

}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------