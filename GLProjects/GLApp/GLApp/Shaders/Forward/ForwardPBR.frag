#version 430 core
#define NUMBER_OF_POINT_LIGHTS 3

//-------------------------------------------------------------------

#include Shaders/HelperShaders/Common.glsl
#include Shaders/HelperShaders/HelperFunctions.glsl
#include Shaders/HelperShaders/PCFShadowCalculation.glsl
#include Shaders/HelperShaders/IBLPBR.glsl

//-------------------------------------------------------------------

in V2F
{
	vec3 Normal;
	vec2 TexCoords;
	vec3 WorldPos;
	vec4 FragPosLightSpace;
	vec3 TangentViewPos;
	vec3 TangentFragPos;
	mat3 TBN;
} fs_in;

out vec4 FragColor;
layout (location = 5) uniform sampler2D ShadowMap;
layout (location = 6) uniform samplerCube IrradenceMap;
layout (location = 7) uniform samplerCube PrefilterMap;
layout (location = 8) uniform sampler2D BrdfLUT;

uniform Material material;
uniform DirLight dirLight;
uniform PointLight pointLights[NUMBER_OF_POINT_LIGHTS];
uniform vec3 CamPos;
uniform vec3 CamDir;

vec3 GetNormalFromMap();

//-------------------------------------------------------------------

vec3 GetNormalFromMap(vec2 InTexCoords)
{
	//normalise it to -1, 1
    vec3 tangentNormal =  texture(material.texture_normal, InTexCoords).rgb * 2.0 - 1.0;
	return normalize(fs_in.TBN * tangentNormal);
 }

//-------------------------------------------------------------------

void main()
{
	//we normalise this result before returning it
	const vec3 View = normalize(CamPos - fs_in.WorldPos);
	const vec3 Norm = GetNormalFromMap(fs_in.TexCoords);
	const float RnMPExponent = 	1.0f;
	LinearMatVals parse = ConvertMapsToPBRValues(material, RnMPExponent, fs_in.TexCoords);

	//Metelic ratio
	vec3 F0 = vec3(0.04);
	F0 = mix(F0, parse.diffuse, parse.metallic);
	
    // reflectance equation (output)
    vec3 L0 = vec3(0.0);

	for(int i = 0; i < NUMBER_OF_POINT_LIGHTS; i++)	
	{
		//per light radiance
		const vec3 PointlightTangentPos = fs_in.TBN * pointLights[i].position;
		const vec3 L = normalize(PointlightTangentPos - fs_in.TangentFragPos);
		const vec3 radiance = pointLights[i].diffuse * CalculateAttenuation(fs_in.TangentFragPos, PointlightTangentPos);	
	//	L0+= ProgrammablePBR(Norm, View, radiance, L, parse, pointLights[i].intensity);
	}
	
	// IBL Ambient
    const vec3 kS = fresnelSchlick(saturate(dot(Norm, View)), F0);
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - parse.metallic;	  
  	const vec3 Irradiance = texture(IrradenceMap, Norm).rgb;
    const vec3 Diffuse = Irradiance * parse.diffuse;
	const vec3 Specular = IBLAmbientSpecular(Norm, View, parse, BrdfLUT, IrradenceMap, PrefilterMap);
    const vec3 Ambient = (kD * Diffuse + Specular) * parse.ao;

	//Directional Lights
	vec3 r = dirLight.diffuse;
	vec3 L = normalize(-dirLight.direction);
	float Shadow = 1.0 - DetermineShadow(fs_in.FragPosLightSpace, Norm, L, ShadowMap);
	L0 += ProgrammablePBR(Norm, View, r, L, parse, dirLight.intensity);
	//L0 *= max(Shadow, 0.01);
	
	vec3 OutputColour = vec3(Ambient + L0); 
	OutputColour *= max(Shadow, 0.025);
	FragColor = vec4(OutputColour, parse.alpha);   
	//FragColor = vec4(vec3(max(Shadow, 0.01)), 1.0f);
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------