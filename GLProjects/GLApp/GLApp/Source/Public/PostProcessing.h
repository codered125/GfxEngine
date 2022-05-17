#pragma once

#include "Source/Public/Math.h"

//-------------------------------------------------------------------

class Shader;
class SceneRenderTarget;

//-------------------------------------------------------------------

enum EffectStatus
{
	UnActive = 0, Active = 1
};

struct PostProcessSettings
{
	EffectStatus InvertedColours, HDR, AntiAliasing, ColourGrading, TimeBasedEffects = EffectStatus::UnActive;
	SceneRenderTarget* MainRenderBuffer;
	SceneRenderTarget* IntermediateRenderBuffer;
	SceneRenderTarget* DepthRenderBuffer;
	SceneRenderTarget* GRenderBuffer;
};

class PostProcessing
{
public:

	static void ApplyEffects(Shader* FBShader, PostProcessSettings* PPS);

	static void TogglePostProcessEffects(int effectNumber, PostProcessSettings* PPS, float& keyboardlockout);
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------