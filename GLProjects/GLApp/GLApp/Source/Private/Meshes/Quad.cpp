#include "Source/Public/Meshes/Quad.h"
#include "Source/Public/Shader.h"
#include "Source/Public/PostProcessing.h"
#include "Source/Public/Rendering/RenderTarget/SceneRenderTarget.h"
#include "Source/Public/Rendering/RenderTexture/RenderTexture.h"

//-------------------------------------------------------------------

#define DEBUGSHADOWMAP 0

//-------------------------------------------------------------------

namespace SV
{
	float PlaneVertices[30] = {
		// positions          // texture Coords 
		5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
		-5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
		-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

		5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
		-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
		5.0f, -0.5f, -5.0f,  2.0f, 2.0f
	};
	float NDCPlaneVertices[24] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// positions		// texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		1.0f, -1.0f,  1.0f, 0.0f,
		1.0f,  1.0f,  1.0f, 1.0f
	};

	float Basic2DQuadVerts[20] = {
		// positions        // texture Coords
		   -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
		   -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	};
}

//-------------------------------------------------------------------

Quad::Quad(Shader* InShader, PostProcessSettings* InPPS, bool InNDC)
{
	glGenVertexArrays(1, &ShapeVAO);
	glGenBuffers(1, &ShapeVBO);

	glBindVertexArray(ShapeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, ShapeVBO);

	//Feels bad need to look into array copy semantics that I actually like
	//glBufferData(GL_ARRAY_BUFFER, sizeof(InNDC ? SV::NDCPlaneVertices : SV::PlaneVertices), &InNDC ? SV::NDCPlaneVertices : SV::PlaneVertices , GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(SV::NDCPlaneVertices), SV::NDCPlaneVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	ShapeID = ShapeVAO;
	ThisShader = InShader;
	ThisPPS = std::make_shared<PostProcessSettings>(*InPPS);
	
}

//-------------------------------------------------------------------

Quad::Quad(Shader* InShader)
{
	glGenVertexArrays(1, &ShapeVAO);
	glGenBuffers(1, &ShapeVBO);

	glBindVertexArray(ShapeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, ShapeVBO);

	//Feels bad need to look into array copy semantics that I actually like
	//glBufferData(GL_ARRAY_BUFFER, sizeof(InNDC ? SV::NDCPlaneVertices : SV::PlaneVertices), &InNDC ? SV::NDCPlaneVertices : SV::PlaneVertices , GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(SV::Basic2DQuadVerts), &SV::Basic2DQuadVerts, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	ThisShader = InShader;
	ShapeID = ShapeVAO;
}

//-------------------------------------------------------------------

void Quad::Draw(glm::mat4 InModel, glm::mat4 InFOV, glm::mat4 InView)
{
	glBindVertexArray(ShapeID);
	//ThisShader->use();

	PostProcessing::ApplyEffects(ThisShader, ThisPPS.get());
	ThisShader->SetSampler("screenTexture", &ThisPPS->IntermediateRenderBuffer->GetColourAttachmentByIndex(0)->GetID(), GL_TEXTURE_2D);

	if (DEBUGSHADOWMAP)
	{
		ThisShader->SetSampler("screenTexture", &ThisPPS->DepthRenderBuffer->GetDepthTexture()->GetID(), GL_TEXTURE_2D);	// use the color attachment texture as the texture of the quad plane
	}
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

//-------------------------------------------------------------------

void Quad::Draw(glm::mat4 InModel, glm::mat4 InFOV, glm::mat4 InView, GLuint* Sampler)
{
	if (Sampler)
	{
		glBindVertexArray(ShapeID);
		ThisShader->use();

		PostProcessing::ApplyEffects(ThisShader, ThisPPS.get());
		ThisShader->setBool("DebugQuad", true);
		ThisShader->SetSampler("screenTexture", Sampler, GL_TEXTURE_2D);


		if (DEBUGSHADOWMAP)
		{
			ThisShader->SetSampler("screenTexture", &ThisPPS->DepthRenderBuffer->GetDepthTexture()->GetID(), GL_TEXTURE_2D);	// use the color attachment texture as the texture of the quad plane
		}
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
	}
	else
	{
		Draw(InModel, InFOV, InView);
	}
}

//-------------------------------------------------------------------

void Quad::Draw(Shader* InShader)
{
	glBindVertexArray(ShapeID);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

//-------------------------------------------------------------------

void Quad::DrawTrianglesStrip(Shader* InShader)
{
	glBindVertexArray(ShapeID);
	InShader->use();
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------


