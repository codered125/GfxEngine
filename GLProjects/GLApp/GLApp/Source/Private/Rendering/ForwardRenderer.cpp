#include "Source/Public/Rendering/ForwardRenderer.h"
#include "Source/Public/SceneRenderTarget.h"
#include "Source/Public/PostProcessing.h"
#include "Source/Public/Shader.h"
#include "Source/Public/Model.h"
#include "Source/Public/Meshes/SkyBox.h"
#include "Source/Public/Meshes/Quad.h"
#include "Source/Public/Camera.h"
#include "Source/Public/Lights/DirectionalLight.h"

#include <vector>
#include <glm.hpp>
#include <gtx\matrix_decompose.hpp>

//-------------------------------------------------------------------

ForwardRenderer::ForwardRenderer(int InScreenWidth, int InScreenHeight) : Renderer(InScreenWidth, InScreenHeight)
{
	MainPostProcessSetting = new PostProcessSettings();
	MainPostProcessSetting->HDR = EffectStatus::Active;

	MainRenderBuffer = new SceneRenderTarget(SCREEN_WIDTH, SCREEN_HEIGHT, GL_TEXTURE_2D_MULTISAMPLE, GL_RGBA16F, GL_UNSIGNED_BYTE, 2, false, true, true);
	MainPostProcessSetting->MainRenderBuffer = MainRenderBuffer;

	IntermediateRenderBuffer= new SceneRenderTarget(SCREEN_WIDTH, SCREEN_HEIGHT, GL_TEXTURE_2D, GL_RGB16F, GL_RGBA, 1, false, false);
	MainPostProcessSetting->IntermediateRenderBuffer = IntermediateRenderBuffer;

	DepthRenderBuffer = new SceneRenderTarget(4096, 4096, GL_TEXTURE_2D, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, 0, true, false);
	MainPostProcessSetting->DepthRenderBuffer = DepthRenderBuffer;

	UnlitShader = new Shader("Shaders/Unlit.vs", "Shaders/Unlit.frag");
	PBRshader = new Shader("Shaders/Forward/ForwardPBR.vs", "Shaders/Forward/ForwardPBR.frag");
	SkyboxShader = new Shader("Shaders/Skybox.vs", "Shaders/Skybox.frag");
	LampShader = new Shader("Shaders/Lamp.vs", "Shaders/Lamp.frag");
	DepthShader = new Shader("Shaders/ShadowMapping.vs", "Shaders/ShadowMapping.frag");
	ScreenShader = new Shader("Shaders/Forward/ForwardScreen.vs", "Shaders/Forward/ForwardScreen.frag");
	WaterShader = new Shader("Shaders/WaterShader.vs", "Shaders/WaterShader.frag", "Shaders/WaterShader");

	Sponza = new Model("Models/SponzaTest/sponza.obj", PBRshader);
	GizMo = new Model("Models/Gizmo/GizmoForMo.obj", UnlitShader);
	WaterBlock = new Model("Models/WaterBlock/SM_bathPoolSurface2.obj", WaterShader);
	if (Tesselation)
	{
		WaterBlock->DrawType = GL_PATCHES;
	}


	VisualSkybox = new SkyBox(SkyboxShader, "Images/KlopHeimCubeMap/", ".png");
	
	PostProcessingQuad = new Quad(ScreenShader, MainPostProcessSetting, true);
}

void ForwardRenderer::RenderLoop(float TimeLapsed)
{
	Renderer::RenderLoop( TimeLapsed);

	//Shadow Render Pass
	glViewport(0, 0, std::get<0>(DepthRenderBuffer->GetDepthTexture()->GetWidthAndHeightOfTexture()), std::get<1>(DepthRenderBuffer->GetDepthTexture()->GetWidthAndHeightOfTexture()));
	glBindFramebuffer(GL_FRAMEBUFFER, DepthRenderBuffer->GetID());
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)
	glCullFace(GL_FRONT);
//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	DepthShader->use();
	RenderDemo(RenderStage::Depth, VisualSkybox, MainCamera, nullptr);

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, MainRenderBuffer->GetID());

	glClearColor(0.05f, 0.05f, 0.05f, 1.0f);// make sure we clear the framebuffer's content
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)	
	//glCullFace(GL_BACK);
	RenderDemo(RenderStage::Main, VisualSkybox, MainCamera, &DepthRenderBuffer->GetDepthTexture()->GetID());

	//Multisampled image contains more informmation than normal images, blits downscales / resolves the image
	//Copies a region from one framebuffer ( our read buffer) to another buffer(our draw buffer)
	glBindFramebuffer(GL_READ_FRAMEBUFFER, MainRenderBuffer->GetID());
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, IntermediateRenderBuffer->GetID());
	glBlitFramebuffer(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GL_COLOR_BUFFER_BIT, GL_NEAREST);

	//PostProcess Render Pass
	// clear all relevant buffers
	glBindFramebuffer(GL_FRAMEBUFFER, 0);// now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
	glClearColor(1.0f, 0.0f, 1.0f, 1.0f); // set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	PostProcessingQuad->ThisShader->use();
	PostProcessingQuad->Draw(glm::mat4(), glm::mat4(), glm::mat4());
}

//-------------------------------------------------------------------

void ForwardRenderer::RenderDemo(RenderStage RenderStage, SkyBox * InSkybox, Camera* Perspective, GLuint * ShadowMap)
{
	//RENDER 
	//skip this for depth pass
	if (RenderStage != RenderStage::Depth)
	{
		InSkybox->Draw(glm::mat4(), Camera::GetProjection(Perspective), Camera::GetViewMatrix(Perspective));
		DrawLights(Perspective, LampShader);


		auto ModelTransformation = glm::mat4();
		ModelTransformation = glm::translate(ModelTransformation, glm::vec3(0.0f, 0.75f, 0.0f));
		ModelTransformation = glm::scale(ModelTransformation, glm::vec3(0.005f));
		DrawWater(WaterShader, WaterBlock, ModelTransformation, MainCamera, GameTimeLapsed);
	//	std::cout << "GameTime: " << GameTimeLapsed << std::endl;
	}

	auto LocalPBRShader = RenderStage == RenderStage::Depth ? DepthShader : PBRshader;
	auto LocalUnlitShader = RenderStage == RenderStage::Depth ? DepthShader : UnlitShader;
	 
	//Room Model
	auto ModelTransformation = glm::mat4();
	ModelTransformation = glm::translate(ModelTransformation, glm::vec3(0.0f, -1.50f, 0.0f));
	ModelTransformation = glm::scale(ModelTransformation, glm::vec3(0.01f));
	ModelTransformation = glm::rotate(ModelTransformation, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	DrawModel(LocalPBRShader, Sponza, ModelTransformation, Perspective, ShadowMap);

	ModelTransformation = glm::mat4();
	ModelTransformation = glm::scale(ModelTransformation, glm::vec3(0.25f));
	ModelTransformation = glm::translate(ModelTransformation, glm::vec3(0.0f, -2.50f, 0.0f));
	//DrawModel(LocalUnlitShader, GizMo, ModelTransformation, Perspective, ShadowMap);

	
}

//-------------------------------------------------------------------

void ForwardRenderer::DrawModel(Shader * ModelShader, Model * InModel, glm::mat4 model, Camera* Perspective, GLuint* ShadowMap)
{
	ModelShader->use();
	if (ShadowMap)
	{
		ModelShader->SetSampler("ShadowMap", ShadowMap, GL_TEXTURE_2D);
	}

	ModelShader->setVec3("CamPos",  Camera::GetPosition(Perspective));
	ModelShader->setVec3("CamDir",  Camera::GetFront(Perspective));

	glm::mat4 FOV = Camera::GetProjection(Perspective);
	glm::mat4 view = Camera::GetViewMatrix(Perspective);
	ModelShader->setMat4("projection", FOV);
	ModelShader->setMat4("view", view);
	InitialiseLightingDataForShader(ModelShader);
	

	if (auto Direction = static_cast<DirectionalLight*>(Directional0))
	{
		glm::mat4 LightingProjection = Direction->GetLightSpaceProjection();
		glm::mat4 LightingView = Direction->GetLightSpaceViewMatrix();
		ModelShader->setMat4("lightSpaceMatrix", LightingProjection * LightingView);
	}
	ModelShader->setMat4("model", model);
	InModel->Draw(ModelShader);
}

//-------------------------------------------------------------------

void ForwardRenderer::DrawWater(Shader* ModelShader, Model* InModel, glm::mat4 model, Camera* Perspective, float TimeLapsed)
{
	ModelShader->use();
	ModelShader->setFloat("TimeLapsed", TimeLapsed);
	ModelShader->setVec3("CamPos", Camera::GetPosition(Perspective));
	ModelShader->setVec3("CamDir", Camera::GetFront(Perspective) );


	glm::vec3 scale;
	glm::quat rotation;
	glm::vec3 translation;
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(model, scale, rotation, translation, skew, perspective);
	ModelShader->setVec3("ActorPos", translation);

	glm::mat4 FOV = Camera::GetProjection(Perspective);
	glm::mat4 View = Camera::GetViewMatrix(Perspective);
	ModelShader->setMat4("projection", FOV);
	ModelShader->setMat4("view", View);
	ModelShader->setMat4("model", model);

	InitialiseLightingDataForShader(ModelShader);
	InModel->Draw(ModelShader);
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------