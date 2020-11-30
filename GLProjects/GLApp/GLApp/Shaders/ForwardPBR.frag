#version 430 core
#define M_PI 3.14159265359
#define NUMBER_OF_POINT_LIGHTS 3

//-------------------------------------------------------------------

#include Shaders/HelperShaders/Common.glsl
#include Shaders/HelperShaders/HelperFunctions.glsl
#include Shaders/HelperShaders/PCFShadowCalculation.glsl
#include Shaders/HelperShaders/PBR.glsl

//-------------------------------------------------------------------

in V2F
{
	vec3 Normal;
	vec2 TexCoords;
	vec3 WorldPos;
	vec4 FragPosLightSpace;
} fs_in;

out vec4 FragColor;

uniform Material material;
uniform DirLight dirLight;
uniform PointLight pointLights[NUMBER_OF_POINT_LIGHTS];
uniform vec3 CamPos;
uniform vec3 CamDir;
uniform sampler2D ShadowMap;

vec3 GetNormalFromMap();

//-------------------------------------------------------------------

vec3 GetNormalFromMap()
{
	//normalise it to 0-1
    vec3 tangentNormal = (texture(material.texture_normal, fs_in.TexCoords).xyz * 2.0) - 1.0;

    vec3 Q1  = dFdx(fs_in.WorldPos);
    vec3 Q2  = dFdy(fs_in.WorldPos);
    vec2 st1 = dFdx(fs_in.TexCoords);
    vec2 st2 = dFdy(fs_in.TexCoords);

    vec3 N   = normalize(fs_in.Normal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}

//-------------------------------------------------------------------

void main()
{
	//we normalise this result before returning it
	vec3 Norm = GetNormalFromMap();
	vec3 View = normalize(CamPos - fs_in.WorldPos);
	float RnMPExponent = 1.0f;
	LinearMatVals parse = ConvertMapsToPBRValues(material, RnMPExponent, fs_in.TexCoords);

	//Metelic ratio
	vec3 F0 = vec3(0.04);
	F0 = mix(F0, parse.diffuse, parse.metallic);
	
    // reflectance equation (output)
    vec3 L0 = vec3(0.0);

	for(int i = 0; i < NUMBER_OF_POINT_LIGHTS; i++)	
	{
		//per light radiance
		vec3 L = normalize(pointLights[i].position - fs_in.WorldPos);
		vec3 Halfway = normalize(View + L);
		vec3 radiance = pointLights[i].diffuse * CalculateAttenuation(fs_in.WorldPos, pointLights[i].position);	
		L0+= ProgrammablePBR(Norm, View, radiance, L, parse, pointLights[i].intensity);
	}

	//Directional Lights
	vec3 r = dirLight.diffuse;
	vec3 L = normalize(dirLight.position - fs_in.WorldPos);
	vec3 ambient = vec3(0.03) * parse.diffuse * parse.ao;
	
	float Shadow = 1.0f - DetermineShadow(fs_in.FragPosLightSpace, normalize(fs_in.Normal), L, ShadowMap);
	L0 += max(Shadow, 0.1) * ProgrammablePBR(Norm, View, r, L, parse, dirLight.intensity);
 	vec3 color = ambient + L0;
	
	//color = color / (color + vec3(1.0));
	//color = pow(color, vec3(1.0/2.2)); 
	FragColor = vec4(color, texture(material.texture_diffuse, fs_in.TexCoords).a);    
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------