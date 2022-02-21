#include "Source/Public/Rendering/ForwardRenderer.h"

#include "Source/Public/Camera.h"
#include "Source/Public/EngineDebugHelper.h"
#include "Source/Public/GlfwInterface.h"
#include "Source/Public/Lights/DirectionalLight.h"
#include "Source/Public/Meshes/Cube.h"
#include "Source/Public/Meshes/Quad.h"
#include "Source/Public/Meshes/SkyBox.h"
#include "Source/Public/Model.h"
#include "Source/Public/PostProcessing.h"
#include "Source/Public/Rendering/RenderTexture/RenderTextureCubeMap.h"
#include "Source/Public/Rendering/RenderTexture/RenderTextureCubeMapIrradence.h"
#include "Source/Public/Rendering/RenderTarget/SceneRenderTarget.h"
#include "Source/Public/Shader.h"

#include <glm.hpp>
#include <gtx\matrix_decompose.hpp>
#include <optional>
#include <vector>

//-------------------------------------------------------------------

ForwardRenderer::ForwardRenderer(int InScreenWidth, int InScreenHeight) : Renderer(InScreenWidth, InScreenHeight)
{
	MainPostProcessSetting = new PostProcessSettings();
	MainPostProcessSetting->HDR = EffectStatus::Active;

	MainRenderBuffer = new SceneRenderTarget(SCREEN_WIDTH, SCREEN_HEIGHT, GL_TEXTURE_2D_MULTISAMPLE, GL_RGBA16F, GL_RGBA, 2, false, true, true);
	MainPostProcessSetting->MainRenderBuffer = MainRenderBuffer;

	IntermediateRenderBuffer= new SceneRenderTarget(SCREEN_WIDTH, SCREEN_HEIGHT, GL_TEXTURE_2D, GL_RGB16F, GL_RGBA, 1, false, false, false);
	MainPostProcessSetting->IntermediateRenderBuffer = IntermediateRenderBuffer;

	DepthRenderBuffer = new SceneRenderTarget(4096, 4096, GL_TEXTURE_2D, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, 0, true, false);
	MainPostProcessSetting->DepthRenderBuffer = DepthRenderBuffer;

	PBRshader = new Shader("Shaders/Forward/ForwardPBR.vs", "Shaders/Forward/ForwardPBR.frag");
	UnlitShader = new Shader("Shaders/Unlit.vs", "Shaders/Unlit.frag");

	SkyboxShader = new Shader("Shaders/Skybox.vs", "Shaders/Skybox.frag");
	LampShader = new Shader("Shaders/Lamp.vs", "Shaders/Lamp.frag");
	DepthShader = new Shader("Shaders/ShadowMapping.vs", "Shaders/ShadowMapping.frag");
	ScreenShader = new Shader("Shaders/Forward/ForwardScreen.vs", "Shaders/Forward/ForwardScreen.frag");
	WaterShader = new Shader("Shaders/WaterShader.vs", "Shaders/WaterShader.frag", "Shaders/WaterShader");

	//Sponza = new Model(GET_VARIABLE_NAME(Sponza), "Models/Room/Room.obj", PBRshader);		// 	MoMessageLogger("Sponza: " + GetGameTimeAsString()); I'll optimise my mesh loading later sponza is the longest thing there
	Sponza = new Model(GET_VARIABLE_NAME(Sponza), "Models/SponzaTest/sponza.obj", PBRshader);		// 	MoMessageLogger("Sponza: " + GetGameTimeAsString()); I'll optimise my mesh loading later sponza is the longest thing there
	//GizMo = new Model(GET_VARIABLE_NAME(GizMo),"Models/Gizmo/GizmoForMo.obj", UnlitShader);
	//WaterBlock = new Model(GET_VARIABLE_NAME(WaterBlock),"Models/WaterBlock/SM_bathPoolSurface2.obj", WaterShader);
	
	IrradenceCapturer = new RenderTextureCubeMapIrradence(GL_TEXTURE_CUBE_MAP, GL_RGB16F, GL_RGB, "Images/HDR.hdr");

	VisualSkybox = new SkyBox(SkyboxShader, "Images/KlopHeimCubeMap/", ".png");
	PostProcessingQuad = new Quad(ScreenShader, MainPostProcessSetting, true);

}

//-------------------------------------------------------------------

void ForwardRenderer::RenderLoop(float TimeLapsed)
{
	Renderer::RenderLoop( TimeLapsed);

	//Begin Shadow Render Pass
	glViewport(0, 0, std::get<0>(DepthRenderBuffer->GetDepthTexture()->GetWidthAndHeightOfTexture()), std::get<1>(DepthRenderBuffer->GetDepthTexture()->GetWidthAndHeightOfTexture()));
	glBindFramebuffer(GL_FRAMEBUFFER, DepthRenderBuffer->GetID());
	GlfwInterface::ResetScreen(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_DEPTH_TEST);
	//glCullFace(GL_FRONT);

	RenderDemo(RenderStage::Depth, VisualSkybox, MainCamera, nullptr);
	//End Shadow Render Pass

	//Begin Main Pass
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, MainRenderBuffer->GetID());
	GlfwInterface::ResetScreen(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_DEPTH_TEST);
	glCullFace(GL_BACK);
	RenderDemo(RenderStage::Main, VisualSkybox, MainCamera, &DepthRenderBuffer->GetDepthTexture()->GetID());
	//DrawGizmos(MainCamera);

	//Multisampled image contains more information than normal images, blits downscales / resolves the image
	//Copies a region from one framebuffer ( our read buffer) to another buffer(our draw buffer)
	glBindFramebuffer(GL_READ_FRAMEBUFFER, MainRenderBuffer->GetID());
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, IntermediateRenderBuffer->GetID());
	glBlitFramebuffer(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	//End Main pass


	//Begin PostProcess Render Pass
	// clear all relevant buffers and disable depth test so screen-space quad isn't discarded due to depth test.
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	GlfwInterface::ResetScreen(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NONE, GL_DEPTH_TEST);
	PostProcessingQuad->ThisShader->use();
	PostProcessingQuad->Draw(glm::mat4(), glm::mat4(), glm::mat4());
	//End PostProcess Render Pass
}

//-------------------------------------------------------------------

void ForwardRenderer::RenderDemo(RenderStage RenderStage, SkyBox* InSkybox, Camera* Perspective, GLuint* ShadowMap)
{
	//RENDER 
	//skip this for depth pass
	if (RenderStage != RenderStage::Depth)
	{
		InSkybox->SkyboxTexture = IrradenceCapturer->GetUnConvolutedRenderTexture();
		InSkybox->Draw(glm::mat4(), Camera::GetProjection(Perspective), Camera::GetViewMatrix(Perspective));
		DrawLights(Perspective, LampShader);
	}

	auto LocalPBRShader = RenderStage == RenderStage::Depth ? DepthShader : PBRshader;
	 
	//Room Model
	auto ModelTransformation = glm::mat4();
	ModelTransformation = glm::translate(ModelTransformation, glm::vec3(0.0f, -2.0f, 2.0f));
	ModelTransformation = glm::scale(ModelTransformation, glm::vec3(0.01f));
//	ModelTransformation = glm::rotate(ModelTransformation, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	DrawModel(LocalPBRShader, Sponza, ModelTransformation, Perspective, ShadowMap);
}

//-------------------------------------------------------------------

void ForwardRenderer::DrawModel(Shader* ModelShader, Shape* InModel, glm::mat4 model, Camera* Perspective, GLuint* ShadowMap)
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
		std::optional<glm::mat4> LightingView = Direction->GetLightSpaceViewMatrix(0);
		if (LightingView.has_value())
		{
			ModelShader->setMat4("lightSpaceMatrix", LightingProjection * LightingView.value());
		}
	}
	ModelShader->setMat4("model", model);
	ModelShader->SetSampler("IrradenceMap", &IrradenceCapturer->GetIrradenceDiffuse()->GetID(), GL_TEXTURE_CUBE_MAP);
	ModelShader->SetSampler("PrefilterMap", &IrradenceCapturer->GetPrefilteredEnvironmentMap()->GetID(), GL_TEXTURE_CUBE_MAP);
	ModelShader->SetSampler("BrdfLUT", &IrradenceCapturer->GetBRDFLookUpTexture()->GetID(), GL_TEXTURE_2D);
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

void ForwardRenderer::DrawGizmos(Camera* Perspective)
{
	Renderer::DrawGizmos(Perspective);
	glDisable(GL_DEPTH_TEST);
	UnlitShader->use();

	auto ModelTransformation = glm::mat4();
	ModelTransformation = glm::scale(ModelTransformation, glm::vec3(0.025f));

	UnlitShader->setVec3("CamPos", Camera::GetPosition(Perspective));
	UnlitShader->setVec3("CamDir", Camera::GetFront(Perspective));
	UnlitShader->setMat4("model", ModelTransformation);

	glm::mat4 FOV = Camera::GetProjection(Perspective);
	glm::mat4 view = glm::mat4(glm::mat3(Camera::GetViewMatrix(Perspective)));	// Remove any translation component of the view matrix
	UnlitShader->setMat4("projection", FOV);
	UnlitShader->setMat4("view", view);
	GizMo->Draw(UnlitShader);
	glEnable(GL_DEPTH_TEST);
}

//-------------------------------------------------------------------

ForwardRenderer::~ForwardRenderer()
{
	if (MainPostProcessSetting)
	{
		delete MainPostProcessSetting;
		MainPostProcessSetting = nullptr;
	}

	if (MainRenderBuffer)
	{
		delete MainRenderBuffer;
		MainRenderBuffer = nullptr;
	}

	if (IntermediateRenderBuffer)
	{
		delete IntermediateRenderBuffer;
		IntermediateRenderBuffer = nullptr;
	}
	
	if (DepthRenderBuffer)
	{
		delete DepthRenderBuffer;
		DepthRenderBuffer = nullptr;
	}

	if (PBRshader)
	{
		delete PBRshader;
		PBRshader = nullptr;
	}

	if (UnlitShader)
	{
		delete UnlitShader;
		UnlitShader = nullptr;
	}

	if (SkyboxShader)
	{
		delete SkyboxShader;
		SkyboxShader = nullptr;
	}

	if (LampShader)
	{
		delete LampShader;
		LampShader = nullptr;
	}

	if (DepthShader)
	{
		delete DepthShader;
		DepthShader = nullptr;
	}

	if (ScreenShader)
	{
		delete ScreenShader;
		ScreenShader = nullptr;
	}

	if (WaterShader)
	{
		delete WaterShader;
		WaterShader = nullptr;
	}

	if (Sponza)
	{
		delete Sponza;
		Sponza = nullptr;
	}

	if (GizMo)
	{
		delete GizMo;
		GizMo = nullptr;
	}

	if (WaterBlock)
	{
		delete WaterBlock;
		WaterBlock = nullptr;
	}

	if (IrradenceCapturer)
	{
		delete IrradenceCapturer;
		IrradenceCapturer = nullptr;
	}

	if (VisualSkybox)
	{
		delete VisualSkybox;
		VisualSkybox = nullptr;
	}

	if (PostProcessingQuad)
	{
		delete PostProcessingQuad;
		PostProcessingQuad = nullptr;
	}

}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------