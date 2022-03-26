#version 430 core

//-------------------------------------------------------------------

struct PostProcessEffects
{
 int Invert;
 int ColourGradiant;
 bool HDR;
};

//-------------------------------------------------------------------

in vec2 TexCoords;
layout (location = 0) out vec4 FragColor; 
layout (location = 1) uniform sampler2D screenTexture;

uniform PostProcessEffects currentPostProcessEffect;

const float exposure = 1.0;
const float offset = 1 /300.0f;
vec3 PostProcessEffect(vec3 untouchedColour);
vec3 ApplyKernal();

//-------------------------------------------------------------------

vec3 PostProcessEffect(vec3 untouchedColour)
{
	vec3 colourOutput = untouchedColour;
	colourOutput = currentPostProcessEffect.Invert ==0? colourOutput :  1 - colourOutput;
	vec3 lerpedColour = (colourOutput.y > colourOutput.x + 0.1) && (colourOutput.y > colourOutput.z + 0.1)? colourOutput + ((vec3(0.5f, 0.0f, 0.0f) - colourOutput) * 0.5) : colourOutput;
	colourOutput = currentPostProcessEffect.ColourGradiant ==0? colourOutput : lerpedColour;
	return colourOutput;
}

//-------------------------------------------------------------------

void main()
{ 	
	const float gamma = 1.f; 
	vec3 hdrColor = texture(screenTexture, TexCoords).rgb;
	if(currentPostProcessEffect.HDR)
    {
        hdrColor *= exposure;
		hdrColor = hdrColor / (hdrColor + vec3(1.0));
        FragColor = vec4(pow(hdrColor, vec3(1.0 / gamma)), 1.0);
    }
    else
    {
        vec3 result = pow( hdrColor, vec3(1.0 / gamma));
        FragColor = vec4( PostProcessEffect(result), 1.0);
    }
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------