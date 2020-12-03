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
} fs_in;

layout (location = 0) out vec3 Position;
layout (location = 1) out vec3 Normal;
layout (location = 2) out vec4 DiffuseShadow;
layout (location = 3) out vec3 NormalMap;
layout (location = 4) out vec3 RMA;

uniform Material material;

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
	float RnMPExponent = 1.0f;
	LinearMatVals parse = ConvertMapsToPBRValues(material, RnMPExponent, fs_in.TexCoords);

	Position = fs_in.WorldPos;
	Normal = fs_in.Normal;
	DiffuseShadow = vec4(parse.diffuse, parse.alpha);
	NormalMap = GetNormalFromMap();
	RMA = vec3(parse.roughness, parse.metallic, parse.ao);
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------