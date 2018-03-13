#version 330 core
out vec4 FragColor;
 
struct PostProcessEffects
{
 int Invert;
 int ColourGradiant;
 bool HDR;
 };
 


in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform float exposure;
uniform PostProcessEffects currentPostProcessEffect;


vec3 PostProcessEffect(vec3 untouchedColour);

void main()
{ 
//const float gamma = 2.2;
const float gamma = 1.0f;
    vec3 hdrColor = texture(screenTexture, TexCoords).rgb;
    if(currentPostProcessEffect.HDR)
    {
        //vec3 result =  hdrColor / (hdrColor + vec3(1.0)); // reinhard
		vec3 result = vec3(1.0) - exp(-hdrColor * exposure); // exposure
        //also gamma correct while we're at it       
        result = pow(result, vec3(1.0 / gamma));
        FragColor = vec4( PostProcessEffect(result), 1.0);
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