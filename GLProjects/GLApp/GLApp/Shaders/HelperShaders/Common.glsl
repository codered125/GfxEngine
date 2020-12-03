#ifndef COMMON
#define COMMON
//-------------------------------------------------------------------

struct DirLight 
{
	float intensity;
	vec3 position;
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

//-------------------------------------------------------------------

struct PointLight 
{
    float constant;
    float linear;
	float intensity;
    float quadratic;

	vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

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
	float alpha;
};
#endif
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------