#include "Source/Public/Rendering/DefferedRenderer.h"

#include "Source/Public/Camera.h"
#include "Source/Public/GlfwInterface.h"
#include "Source/Public/Lights/DirectionalLight.h"
#include "Source/Public/Meshes/Quad.h"
#include "Source/Public/Meshes/SkyBox.h"
#include "Source/Public/Model.h"
#include "Source/Public/PostProcessing.h"
#include "Source/Public/Rendering/RenderingHelpers/RenderTextureSSAO.h"
#include "Source/Public/Rendering/RenderTarget/SceneRenderTarget.h"
#include "Source/Public/Rendering/RenderTexture/RenderTextureCubeMapIrradence.h"
#include "Source/Public/Shader.h"

#include <glm.hpp>
#include <gtx\matrix_decompose.hpp>
#include <vector>


//-------------------------------------------------------------------

DefferedRenderer::DefferedRenderer(int InScreenWidth, int InScreenHeight) : Renderer(InScreenWidth, InScreenHeight)
{
	MainPostProcessSetting = new PostProcessSettings();
	MainPostProcessSetting->HDR = EffectStatus::Active;

	MainRenderBuffer = new SceneRenderTarget(SCREEN_WIDTH, SCREEN_HEIGHT, GL_TEXTURE_2D_MULTISAMPLE, GL_RGBA16F, GL_UNSIGNED_BYTE, 2, false, true);
	MainPostProcessSetting->MainRenderBuffer = MainRenderBuffer;

	IntermediateRenderBuffer = new SceneRenderTarget(SCREEN_WIDTH, SCREEN_HEIGHT, GL_TEXTURE_2D, GL_RGB16F, GL_RGBA, 1, false, false);
	MainPostProcessSetting->IntermediateRenderBuffer = IntermediateRenderBuffer;

	DepthRenderBuffer = new SceneRenderTarget(8192, 8192, GL_TEXTURE_2D, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, 0, true, false);
	MainPostProcessSetting->DepthRenderBuffer = DepthRenderBuffer;

	GBuffer = new SceneRenderTarget(SCREEN_WIDTH, SCREEN_HEIGHT, GL_TEXTURE_2D, GL_RGBA16F, GL_RGBA, 6, false, false, true);
	MainPostProcessSetting->GRenderBuffer = GBuffer;

	SSAOBuilder = new RenderTextureSSAO(SCREEN_WIDTH, SCREEN_HEIGHT);

	UnlitShader = new Shader("Shaders/Unlit.vs", "Shaders/Unlit.frag");
	SkyboxShader = new Shader("Shaders/Skybox.vs", "Shaders/Skybox.frag");
	LampShader = new Shader("Shaders/Lamp.vs", "Shaders/Lamp.frag");
	DepthShader = new Shader("Shaders/ShadowMapping.vs", "Shaders/ShadowMapping.frag");
	ScreenShader = new Shader("Shaders/Deffered/DefferedScreen.vs", "Shaders/Deffered/DefferedScreen.frag");
	GBufferShader = new Shader("Shaders/Deffered/DefferedGBufferFill.vs", "Shaders/Deffered/DefferedGBufferFill.frag");

	Sponza = new Model(GET_VARIABLE_NAME(Sponza), "Models/SponzaTest/sponza.obj", PBRshader);		// 	MoMessageLogger("Sponza: " + GetGameTimeAsString()); I'll optimise my mesh loading later sponza is the longest thing there
	IrradenceCapturer = new RenderTextureCubeMapIrradence(GL_TEXTURE_CUBE_MAP, GL_RGB16F, GL_RGB, "Images/HDR.hdr");
	VisualSkybox = new SkyBox(SkyboxShader, "Images/KlopHeimCubeMap/", ".png");
	PostProcessingQuad = new Quad(ScreenShader, MainPostProcessSetting, true);
}

void DefferedRenderer::RenderLoop( float TimeLapsed)
{
	Renderer::RenderLoop(TimeLapsed);

	//Begin Shadow Render Pass
	glViewport(0, 0, std::get<0>(DepthRenderBuffer->GetDepthTexture()->GetWidthAndHeightOfTexture()), std::get<1>(DepthRenderBuffer->GetDepthTexture()->GetWidthAndHeightOfTexture()));
	glBindFramebuffer(GL_FRAMEBUFFER, DepthRenderBuffer->GetID());
	GlfwInterface::ResetScreen(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_DEPTH_TEST);
	glCullFace(GL_FRONT);
	RenderDemo(RenderStage::Depth, VisualSkybox, MainCamera, nullptr);
	//End Shadow Render Pass

	//Begin Main Pass
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, GBuffer->GetID());
	GlfwInterface::ResetScreen(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_DEPTH_TEST);
	glCullFace(GL_BACK);
	RenderDemo(RenderStage::GBuffer, VisualSkybox, MainCamera, &DepthRenderBuffer->GetDepthTexture()->GetID());

	//Multisampled image contains more informmation than normal images, blits downscales / resolves the image
	//Copies a region from one framebuffer ( our read buffer) to another buffer(our draw buffer)
	glBindFramebuffer(GL_READ_FRAMEBUFFER, GBuffer->GetID());
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, IntermediateRenderBuffer->GetID());
	glBlitFramebuffer(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	glBlitFramebuffer(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	//End Main pass


	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	SSAOBuilder->RenderCommandsColour(GBuffer->GetColourAttachmentByIndex(0), GBuffer->GetColourAttachmentByIndex(1), Camera::GetProjection(MainCamera), Camera::GetViewMatrix(MainCamera));
	SSAOBuilder->RenderCommandsBlur(SSAOBuilder->GetSSAOColourBuffer()->GetColourAttachmentByIndex(0));
	 

	//Begin Lighting Render Pass
	// clear all relevant buffers
	glBindFramebuffer(GL_FRAMEBUFFER, 0);// now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
	GlfwInterface::ResetScreen(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NONE, GL_DEPTH_TEST);
	PostProcessingQuad->ThisShader->use();
	PostProcessingQuad->ThisShader->setVec3("CamPos",  Camera::GetPosition(MainCamera));
	PostProcessingQuad->ThisShader->setVec3("CamDir",  Camera::GetFront(MainCamera));
	PostProcessingQuad->ThisShader->SetSampler("PositionTexture", &GBuffer->GetColourAttachmentByIndex(0)->GetID(), GL_TEXTURE_2D);
	PostProcessingQuad->ThisShader->SetSampler("NormalTexture", &GBuffer->GetColourAttachmentByIndex(1)->GetID(), GL_TEXTURE_2D);
	PostProcessingQuad->ThisShader->SetSampler("DiffuseShadowTexture", &GBuffer->GetColourAttachmentByIndex(2)->GetID(), GL_TEXTURE_2D);
	PostProcessingQuad->ThisShader->SetSampler("NormalMapTexture", &GBuffer->GetColourAttachmentByIndex(3)->GetID(), GL_TEXTURE_2D);
	PostProcessingQuad->ThisShader->SetSampler("RMATexture", &GBuffer->GetColourAttachmentByIndex(4)->GetID(), GL_TEXTURE_2D);
	PostProcessingQuad->ThisShader->SetSampler("FragPosLightSpaceTexture", &GBuffer->GetColourAttachmentByIndex(5)->GetID(), GL_TEXTURE_2D);

	PostProcessingQuad->ThisShader->SetSampler("ShadowMap", &DepthRenderBuffer->GetDepthTexture()->GetID(), GL_TEXTURE_2D);
	PostProcessingQuad->ThisShader->SetSampler("IrradenceMap", &IrradenceCapturer->GetIrradenceDiffuse()->GetID(), GL_TEXTURE_CUBE_MAP);
	PostProcessingQuad->ThisShader->SetSampler("PrefilterMap", &IrradenceCapturer->GetPrefilteredEnvironmentMap()->GetID(), GL_TEXTURE_CUBE_MAP);
	PostProcessingQuad->ThisShader->SetSampler("BrdfLUT", &IrradenceCapturer->GetBRDFLookUpTexture()->GetID(), GL_TEXTURE_2D);
	PostProcessingQuad->ThisShader->SetSampler("SSAOTexture", &SSAOBuilder->GetSSAOBlurBuffer()->GetColourAttachmentByIndex(0)->GetID(), GL_TEXTURE_2D);

	if (auto Direction = static_cast<DirectionalLight*>(Directional0))
	{
		glm::mat4 LightingProjection = Direction->GetLightSpaceProjection();
		std::optional<glm::mat4> LightingView = Direction->GetLightSpaceViewMatrix(0);
		if (LightingView.has_value())
		{
			PostProcessingQuad->ThisShader->setMat4("lightSpaceMatrix", LightingProjection * LightingView.value());
		}
	}

	InitialiseLightingDataForShader(PostProcessingQuad->ThisShader);
	RenderTexture* OutputTexture = SSAOBuilder->GetSSAOBlurBuffer()->GetColourAttachmentByIndex(0);
	PostProcessingQuad->Draw(glm::mat4(), glm::mat4(), glm::mat4(), &OutputTexture->GetID());
	//PostProcessingQuad->Draw(glm::mat4(), glm::mat4(), glm::mat4());


	//End Lighting Render Pass
}

//-------------------------------------------------------------------

void DefferedRenderer::RenderDemo(RenderStage RenderStage, SkyBox * InSkybox, Camera* Perspective, GLuint * ShadowMap)
{
	//RENDER 
	//skip this for depth pass
	if (RenderStage != RenderStage::Depth)
	{
		//InSkybox->SkyboxTexture = IrradenceCapturer->GetUnConvolutedRenderTexture();
		//InSkybox->Draw(glm::mat4(), Camera::GetProjection(Perspective), Camera::GetViewMatrix(Perspective));
		//DrawLights(Perspective, nullptr);
	}

	auto LocalPBRShader = RenderStage == RenderStage::Depth ? DepthShader : GBufferShader;
	 
	//Room Model
	auto ModelTransformation = glm::mat4();
	ModelTransformation = glm::translate(ModelTransformation, glm::vec3(0.0f, 2.0f, -2.0f));
	ModelTransformation = glm::scale(ModelTransformation, glm::vec3(0.01f));
	DrawModel(LocalPBRShader, Sponza, ModelTransformation, Perspective, ShadowMap);
}


//-------------------------------------------------------------------

void DefferedRenderer::DrawModel(Shader * ModelShader, Model * InModel, glm::mat4 model, Camera * Perspective, GLuint * ShadowMap)
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
	InModel->Draw(ModelShader);
}

//-------------------------------------------------------------------

DefferedRenderer::~DefferedRenderer()
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

	if (GBuffer)
	{
		delete GBuffer;
		GBuffer = nullptr;
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