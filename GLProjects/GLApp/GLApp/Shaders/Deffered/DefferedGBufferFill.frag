#version 430 core
#define M_PI 3.14159265359
//-------------------------------------------------------------------

#include Shaders/HelperShaders/Common.glsl
#include Shaders/HelperShaders/HelperFunctions.glsl
#include Shaders/HelperShaders/PBR.glsl

//-------------------------------------------------------------------

in V2F
{
	vec3 Normal;
	vec2 TexCoords;
	vec3 WorldPos;
	vec4 FragPosLightSpace;
	mat3 TBN;
} fs_in;

layout (location = 0) out vec3 Position;
layout (location = 1) out vec3 Normal;
layout (location = 2) out vec4 DiffuseShadow;
layout (location = 3) out vec3 NormalMap;
layout (location = 4) out vec3 RMA;
layout (location = 5) out vec4 FragPosLightSpaceTexture;

uniform Material material;

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
	float RnMPExponent = 1.0f;
	LinearMatVals parse = ConvertMapsToPBRValues(material, RnMPExponent, fs_in.TexCoords);

	Position = fs_in.WorldPos;
	Normal = fs_in.Normal;
	DiffuseShadow = vec4(parse.diffuse, parse.alpha);
	NormalMap = GetNormalFromMap();
	FragPosLightSpaceTexture = fs_in.FragPosLightSpace;
	RMA = vec3(parse.roughness, parse.metallic, parse.ao);

}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------