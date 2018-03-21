#pragma once
#include "Shader.h"
class PostProcessing
{
public:

	enum EffectStatus
	{
		UnActive, Active
	};

	static struct PostProcessSettings
	{
		EffectStatus InvertedColours, HDR, AntiAliasing, ColourGrading, TimeBasedEffects = EffectStatus::UnActive;
	};

	static void ApplyEffects(Shader * FBShader, PostProcessSettings &PPS)
	{
		FBShader->use();
		FBShader->setInt("screenTexture", 0);
		FBShader->setBool("currentPostProcessEffect.HDR", PPS.HDR);
		FBShader->setFloat("exposure", 0.50f);
		FBShader->setInt("currentPostProcessEffect.Invert", PPS.InvertedColours);
		FBShader->setInt("currentPostProcessEffect.ColourGradiant", PPS.ColourGrading);
	};


};