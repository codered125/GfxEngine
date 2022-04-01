#version 430 core
#define M_PI 3.14159265359
#define NUMBER_OF_POINT_LIGHTS 3

//-------------------------------------------------------------------

struct Material
{
    sampler2D  texture_diffuse;
    sampler2D  texture_specular;
	sampler2D  texture_normal;
	sampler2D  texture_height;
	sampler2D  texture_roughness;
	sampler2D  texture_metallic;
	sampler2D  texture_ao;
	float shininess;
};

//-------------------------------------------------------------------

struct LinearMatVals
{
	float roughness;
	float metallic;
    float ao;
	vec3 diffuse;
};

//-------------------------------------------------------------------

in vec2 TexCoords;
in vec3 Normal;
in vec3 WorldPos;

out vec4 color;

uniform Material material;

uniform vec3 CamPos;
uniform vec3 CamDir;
bool  blin = false;

vec3 GetNormalFromMap();

//-------------------------------------------------------------------

vec3 GetNormalFromMap()
{
	//normalise it to 0-1
    vec3 tangentNormal = (texture(material.texture_normal, TexCoords).xyz * 2.0) - 1.0;

    vec3 Q1  = dFdx(WorldPos);
    vec3 Q2  = dFdy(WorldPos);
    vec2 st1 = dFdx(TexCoords);
    vec2 st2 = dFdy(TexCoords);

    vec3 N   = normalize(Normal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}

//-------------------------------------------------------------------

void main()
{
	color = vec4(texture(material.texture_diffuse, TexCoords).xyz, 1.0f);
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------