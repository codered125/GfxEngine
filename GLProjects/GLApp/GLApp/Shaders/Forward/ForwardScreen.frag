#version 430 core

struct PostProcessEffects
{
 int Invert;
 int ColourGradiant;
 bool HDR;
};

in vec2 TexCoords;

layout (location = 0) out vec4 FragColor;
 
layout (location = 1) uniform sampler2D screenTexture;
uniform PostProcessEffects currentPostProcessEffect;

const float exposure = 1.0;
const float offset = 1 /300.0f;

vec3 PostProcessEffect(vec3 untouchedColour);
vec3 ApplyKernal();

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

vec3 PostProcessEffect(vec3 untouchedColour)
{
	vec3 colourOutput = untouchedColour;
	colourOutput = currentPostProcessEffect.Invert ==0? colourOutput :  1 - colourOutput;
	vec3 lerpedColour = (colourOutput.y > colourOutput.x + 0.1) && (colourOutput.y > colourOutput.z + 0.1)? colourOutput + ((vec3(0.5f, 0.0f, 0.0f) - colourOutput) * 0.5) : colourOutput;
	colourOutput = currentPostProcessEffect.ColourGradiant ==0? colourOutput : lerpedColour;
	return colourOutput;
}
vec3 ApplyKernal()
{


	vec2 offsets[9] = vec2[](
		vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right 
		);

	float kernel[9] = float[](
		1.0 / 16, 2.0 / 16, 1.0 / 16,
		2.0 / 16, 4.0 / 16, 2.0 / 16,
		1.0 / 16, 2.0 / 16, 1.0 / 16  
	);


	vec3 sampleTex[9];
	for (int i = 0; i < 9; i++)
	{

	 sampleTex[i] =  vec3(texture(screenTexture, TexCoords + offsets[i]));
	}
	vec3 outputcol;
	for (int i = 0 ; i < 9; i++)
	{
	outputcol += sampleTex[i] * kernel[i];
	}
	
	return outputcol;

}
