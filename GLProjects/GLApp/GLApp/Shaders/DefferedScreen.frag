#version 430 core

#include Shaders/HelperShaders/Common.glsl
#include Shaders/HelperShaders/HelperFunctions.glsl
#include Shaders/HelperShaders/PCFShadowCalculation.glsl
#include Shaders/HelperShaders/PBR.glsl

#define NUMBER_OF_POINT_LIGHTS 3


struct PostProcessEffects
{
 int Invert;
 int ColourGradiant;
 bool HDR;
};

in vec2 TexCoords;

layout (location = 0) out vec4 FragColor;
 
layout (location = 1) uniform sampler2D PositionTexture;
layout (location = 2) uniform sampler2D NormalTexture;
layout (location = 3) uniform sampler2D DiffuseShadowTexture;
layout (location = 4) uniform sampler2D NormalMapTexture;
layout (location = 5) uniform sampler2D RMATexture;
layout (location = 6) uniform sampler2D ShadowMap;

uniform PostProcessEffects currentPostProcessEffect;
uniform DirLight dirLight;
uniform PointLight pointLights[NUMBER_OF_POINT_LIGHTS];
uniform vec3 CamPos;
uniform vec3 CamDir;

const float exposure = 1.5;
const float offset = 1 /300.0f;

vec3 PostProcessEffect(vec3 untouchedColour);
vec3 ApplyKernal();
vec4 CalculateLight();

void main()
{ 	
	const float gamma = 1.0f; //const float gamma = 2.2f;
	vec3 hdrColor = CalculateLight().rgb;

	if(currentPostProcessEffect.HDR)
    {
        //vec3 result = hdrColor / (hdrColor + vec3(1.0)); // reinhard
		vec3 result = vec3(1.0) - exp(-hdrColor * exposure); // exposure
        //gamma correction       
        result = pow(result, vec3(1.0 / gamma));
        FragColor = vec4( PostProcessEffect(result), 1.0);
	   // float brightness = dot(normalize(FragColor.rgb),  vec3(0.2126, 0.7152, 0.0722));
		//FragColor = brightness > 01.0? vec4(ApplyKernal(), 1.0f) : FragColor;// vec4(ApplyKernal(), 1.0f) : FragColor;	
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

//	 sampleTex[i] =  vec3(texture(screenTexture, TexCoords + offsets[i]));
	}
	vec3 outputcol;
	for (int i = 0 ; i < 9; i++)
	{
	outputcol += sampleTex[i] * kernel[i];
	}
	
	return outputcol;
}

vec4 CalculateLight()
{
	vec3 FragPos = texture(PositionTexture, TexCoords).rgb;
	vec3 FragNormal = texture(NormalTexture, TexCoords).rgb;
	vec4 FragDiffuseShadow = texture(DiffuseShadowTexture, TexCoords).rgba;
	vec3 FragNormalMap = texture(NormalMapTexture, TexCoords).rgb;
	vec3 FragRMA = texture(RMATexture, TexCoords).rgb;
	LinearMatVals Parse = LinearMatVals(FragRMA.x, FragRMA.y, FragRMA.z, FragDiffuseShadow.rgb, FragDiffuseShadow.a);

	vec3 View = normalize(CamPos - FragPos);

	//Metelic ratio
	vec3 F0 = vec3(0.04);
	F0 = mix(F0, Parse.diffuse, Parse.metallic);
	
    // reflectance equation (output)
    vec3 L0 = vec3(0.0);

	for(int i = 0; i < NUMBER_OF_POINT_LIGHTS; i++)	
	{
		//per light radiance
		vec3 L = normalize(pointLights[i].position - FragPos);
		vec3 Halfway = normalize(View + L);
		vec3 radiance = pointLights[i].diffuse * CalculateAttenuation(FragPos, pointLights[i].position);	
		L0+= ProgrammablePBR(FragNormalMap, View, radiance, L, Parse, pointLights[i].intensity);
	}

	//Directional Lights
	vec3 r = dirLight.diffuse;
	vec3 L = normalize(-dirLight.direction);
	vec3 ambient = vec3(0.03) * Parse.diffuse * Parse.ao;
	
	//float Shadow = 1.0f - DetermineShadow(fs_in.FragPosLightSpace, normalize(FragNormal), L, ShadowMap);
	L0 += ProgrammablePBR(FragNormalMap, View, r, L, Parse, dirLight.intensity);
 	vec3 color = ambient + L0;
	
	//color = color / (color + vec3(1.0));
	//color = pow(color, vec3(1.0/2.2)); 
	return vec4(color, Parse.alpha);    
}