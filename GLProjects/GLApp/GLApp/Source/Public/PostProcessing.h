#pragma once

#include "Source/Public/Math.h"

//-------------------------------------------------------------------

class Shader;

//-------------------------------------------------------------------

enum EffectStatus
{
	UnActive = 0, Active = 1
};

struct PostProcessSettings
{
	EffectStatus InvertedColours, HDR, AntiAliasing, ColourGrading, TimeBasedEffects = EffectStatus::UnActive;
};

class PostProcessing
{
public:

	static void ApplyEffects(Shader * FBShader, PostProcessSettings &PPS);

	static void TogglePostProcessEffects(int effectNumber, PostProcessSettings * PPS, GLfloat& keyboardlockout);
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------