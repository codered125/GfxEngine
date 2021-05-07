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
	mat3 TBN;
} fs_in;

out vec4 FragColor;
layout (location = 5) uniform sampler2D ShadowMap;
layout (location = 6) uniform samplerCube IrradenceMap;

uniform Material material;
uniform DirLight dirLight;
uniform PointLight pointLights[NUMBER_OF_POINT_LIGHTS];
uniform vec3 CamPos;
uniform vec3 CamDir;

vec3 GetNormalFromMap();

//-------------------------------------------------------------------

vec3 GetNormalFromMap()
{
	//normalise it to -1, 1
    vec3 tangentNormal = ((texture(material.texture_normal, fs_in.TexCoords).rgb * 2.0) - 1.0);
	return normalize(fs_in.TBN * tangentNormal);
 }

//-------------------------------------------------------------------

void main()
{
	//we normalise this result before returning it
	const vec3 Norm = GetNormalFromMap();
	const vec3 View = normalize(CamPos - fs_in.WorldPos);
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
		const vec3 L = normalize(pointLights[i].position - fs_in.WorldPos);
		const vec3 radiance = pointLights[i].diffuse * CalculateAttenuation(fs_in.WorldPos, pointLights[i].position);	
		L0+= ProgrammablePBR(Norm, View, radiance, L, parse, pointLights[i].intensity);
	}
	
	// IBL Ambient
    const vec3 kS = fresnelSchlick(saturate(dot(Norm, View)), F0);
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - parse.metallic;	  
  	const vec3 Irradiance = texture(IrradenceMap, Norm).rgb;
    const vec3 Diffuse = Irradiance * parse.diffuse;
    const vec3 Ambient = (kD * Diffuse) * parse.ao;

	//Directional Lights
	vec3 r = dirLight.diffuse;
	vec3 L = normalize(-dirLight.direction);
	float Shadow = 1.0 - DetermineShadow(fs_in.FragPosLightSpace, Norm, L, ShadowMap);
	L0 += (ProgrammablePBR(Norm, View, r, L, parse, dirLight.intensity) *max(Shadow, 0.1)) ;
 	
	const vec3 OutputColour = vec3(L0 + Ambient); 
	FragColor = vec4(OutputColour, parse.alpha);   
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------