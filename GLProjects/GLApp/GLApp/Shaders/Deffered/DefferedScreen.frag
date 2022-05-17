#version 430 core
#define NUMBER_OF_POINT_LIGHTS 3

//-------------------------------------------------------------------

#include Shaders/HelperShaders/Common.glsl
#include Shaders/HelperShaders/HelperFunctions.glsl
#include Shaders/HelperShaders/PCFShadowCalculation.glsl
#include Shaders/HelperShaders/IBLPBR.glsl

//-------------------------------------------------------------------

struct PostProcessEffects
{
 int Invert;
 int ColourGradiant;
 bool HDR;
};

//-------------------------------------------------------------------

in vec2 TexCoords;
out vec4 FragColor;

layout (location = 2) uniform sampler2D PositionTexture;
layout (location = 3) uniform sampler2D NormalTexture;
layout (location = 4) uniform sampler2D DiffuseShadowTexture;
layout (location = 5) uniform sampler2D NormalMapTexture;
layout (location = 6) uniform sampler2D RMATexture;
layout (location = 7) uniform sampler2DArray ShadowMaps;
layout (location = 8) uniform sampler2D FragPosLightSpaceTexture;
layout (location = 9) uniform samplerCube IrradenceMap;
layout (location = 10) uniform samplerCube PrefilterMap;
layout (location = 11) uniform sampler2D BrdfLUT;
layout (location = 12) uniform sampler2D screenTexture;

uniform PostProcessEffects currentPostProcessEffect;
uniform DirLight dirLight;
uniform PointLight pointLights[NUMBER_OF_POINT_LIGHTS];
uniform vec3 CamPos;
uniform vec3 CamDir;
uniform mat4 LightSpaceMatricesArray[10];

uniform mat4 ViewMatrix;
uniform bool DebugQuad;
uniform sampler2D SSAOTexture;
uniform float CascadingLevelsArray[10];
uniform int CascadeCount; 

const float exposure = 1.5;
const float offset = 1 /300.0f;

vec3 PostProcessEffect(vec3 untouchedColour);
vec3 ApplyKernal();
vec4 CalculateLight();

//-------------------------------------------------------------------

void main()
{ 	
	const float gamma = 1.0f; //const float gamma = 2.2f;
	vec3 hdrColor = CalculateLight().rgb;
	//if(DebugQuad)
	{
		FragColor = vec4(hdrColor, 1.0f);
		return;
	}
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

vec3 PostProcessEffect(vec3 untouchedColour)
{
	vec3 colourOutput = untouchedColour;
	colourOutput = currentPostProcessEffect.Invert ==0? colourOutput :  1 - colourOutput;
	vec3 lerpedColour = (colourOutput.y > colourOutput.x + 0.1) && (colourOutput.y > colourOutput.z + 0.1)? colourOutput + ((vec3(0.5f, 0.0f, 0.0f) - colourOutput) * 0.5) : colourOutput;
	colourOutput = currentPostProcessEffect.ColourGradiant ==0? colourOutput : lerpedColour;
	return colourOutput;
}

//-------------------------------------------------------------------

vec4 CalculateLight()
{
	const vec3 FragPos = texture(PositionTexture, TexCoords).rgb;
	const vec3 FragNormal = texture(NormalTexture, TexCoords).rgb;
	const vec4 FragDiffuseShadow = texture(DiffuseShadowTexture, TexCoords).rgba;
	const vec3 FragNormalMap = texture(NormalMapTexture, TexCoords).rgb;
	const vec3 FragRMA = texture(RMATexture, TexCoords).rgb;
	const vec4 FragPosLightSpace2 = texture(FragPosLightSpaceTexture, TexCoords).rgba;
	const float AmbientOcclusion = texture(SSAOTexture, TexCoords).r;
	LinearMatVals Parse = LinearMatVals(FragRMA.x, FragRMA.y, FragRMA.z, FragDiffuseShadow.rgb, FragDiffuseShadow.a);

	const vec3 View = normalize(CamPos - FragPos);

	//Metelic ratio
	vec3 F0 = vec3(0.04);
	F0 = mix(F0, Parse.diffuse, Parse.metallic);
	
    // reflectance equation (output)
    vec3 L0 = vec3(0.0);
	for(int i = 0; i < NUMBER_OF_POINT_LIGHTS; i++)	
	{
		//per light radiance
		const vec3 L = normalize(pointLights[i].position - FragPos);
		const vec3 Halfway = normalize(View + L);
		const vec3 radiance = pointLights[i].diffuse * CalculateAttenuation(FragPos, pointLights[i].position);	
		//L0+= ProgrammablePBR(FragNormalMap, View, radiance, L, Parse, pointLights[i].intensity);
	}

	// IBL Ambient
    const vec3 kS = fresnelSchlick(saturate(dot(FragNormalMap, View)), F0);
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - Parse.metallic;	  
  	const vec3 Irradiance = texture(IrradenceMap, FragNormalMap).rgb;
    const vec3 Diffuse = Irradiance * Parse.diffuse;
	const vec3 Specular = IBLAmbientSpecular(FragNormalMap, View, Parse, BrdfLUT, IrradenceMap, PrefilterMap);
    const vec3 Ambient = (kD * Diffuse + Specular) * Parse.ao;


	//Directional Lights
	const vec3 r = dirLight.diffuse;
	const vec3 L = normalize(-dirLight.direction);
	//const float Shadow = 1.0f - DetermineShadow(FragPosLightSpace, FragNormalMap, L, ShadowMap);

	int Layer = GetCascadingLayer(vec4(FragPos, 1.0f), ViewMatrix, CascadingLevelsArray, CascadeCount);
	//Layer = 1;
	const vec4 FragPosLightSpace = LightSpaceMatricesArray[Layer] * vec4(FragPos, 1.0f);
	const float Shadow = 1.0f - DetermineShadowCSM(FragPosLightSpace, normalize(FragNormalMap), L, ShadowMaps, Layer, CascadingLevelsArray,  CascadeCount, CameraFarPlane);
	//if(length(FragPos) != 0)
	{
		//return vec4(Shadow);
		//return vec4( 1 / FragPosLightSpace);
	}
	L0 += ProgrammablePBR(FragNormalMap, View, r, L, Parse, dirLight.intensity);

	vec3 OutputColour = vec3(Ambient + L0); 
	OutputColour *= max(Shadow, 0.05);
	OutputColour *= max(AmbientOcclusion, 0.025);
	if(DebugQuad)
	{
		return vec4(texture(ShadowMaps, vec3(TexCoords, 1)).r * vec4(1.0f));
	}
	return vec4( OutputColour, 1.0f); 
  
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------