#pragma once

#include "Source/Public/PostProcessing.h"
#include "Source/Public/Shader.h"

//-------------------------------------------------------------------

void PostProcessing::ApplyEffects(Shader* FBShader, PostProcessSettings *PPS)
{
//	FBShader->use();
	if (!PPS || !FBShader)
	{
		return;
	}
	FBShader->setBool("currentPostProcessEffect.HDR", PPS->HDR);
	FBShader->setFloat("exposure", 0.50f);
	FBShader->setInt("currentPostProcessEffect.Invert", PPS->InvertedColours);
	FBShader->setInt("currentPostProcessEffect.ColourGradiant", PPS->ColourGrading);
};

//-------------------------------------------------------------------

void PostProcessing::TogglePostProcessEffects(int effectNumber, PostProcessSettings * PPS, GLfloat & keyboardlockout)
{
	keyboardlockout = 0.1f;
	switch (effectNumber)
	{
	case 1:
		PPS->InvertedColours = static_cast<EffectStatus>(!PPS->InvertedColours == 1);
		break;
	case 2:
		PPS->HDR = static_cast<EffectStatus>(!PPS->HDR == 1);
		break;
	case 3:
		PPS->ColourGrading = static_cast<EffectStatus>(!PPS->ColourGrading == 1);
		break;
	case 4:
		PPS->TimeBasedEffects = static_cast<EffectStatus>(!PPS->TimeBasedEffects == 1);
		break;
	}
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------