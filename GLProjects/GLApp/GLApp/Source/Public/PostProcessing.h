#pragma once
#include "Shader.h"
class PostProcessing
{
public:

	enum EffectStatus
	{
		UnActive, Active
	};

	struct PostProcessSettings
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

	static void TogglePostProcessEffects(int effectNumber, PostProcessSettings * PPS, GLfloat & keyboardlockout)
	{
		keyboardlockout = 0.1f;
		switch (effectNumber)
		{
		case 1:
			PPS->InvertedColours = static_cast<PostProcessing::EffectStatus>(PPS->InvertedColours == 1 ? 0 : 1);
			break;
		case 2:
			PPS->HDR = static_cast<PostProcessing::EffectStatus>(PPS->HDR == 1 ? 0 : 1);
			break;
		case 3:
			PPS->ColourGrading = static_cast<PostProcessing::EffectStatus>(PPS->ColourGrading == 1 ? 0 : 1);
			break;
		case 4:
			PPS->TimeBasedEffects = static_cast<PostProcessing::EffectStatus>(PPS->TimeBasedEffects == 1 ? 0 : 1);
			break;
		}
	}
};