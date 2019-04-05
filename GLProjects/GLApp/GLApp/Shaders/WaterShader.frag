#version 330 core
#define M_PI 3.14159265359
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
struct LinearMatVals
{
	float roughness;
	float metallic;
    float ao;
	vec3 diffuse;
};
struct DirLight 
{
	float intensity;

	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;
in vec3 WorldPos;


out vec4 color;

uniform vec3 CamPos;
uniform vec3 CamDir;
uniform float TimeLapsed;
uniform samplerCube skybox;
uniform Material material;
uniform vec3 ActorPos;
uniform DirLight dirLight;

float saturate(float x) {return max(min(x, 1.0f), 0.0f);};
float UE4SphereMask(vec3 a, vec3 b, float Radius, float Hardness);
vec3 getNormalFromMap();
vec2 Panner(float XSpeed, float YSpeed);

vec3 GetNormalFromMap();
float DistributionGGX(vec3 Norm, vec3 Halfway, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 Norm, vec3 View, vec3 L, float roughness);
vec3 fresnelSchlick(float cosTheta, vec3 F0, float roughness);
vec3 ProgrammablePBR(vec3 Norm, vec3 View, vec3 Radiance, vec3 L, LinearMatVals ToParse, float intensity);

vec2 Panner(float XSpeed, float YSpeed)
{
	vec2 output;
	output.x = TexCoords.x + ( XSpeed * TimeLapsed);
	output.y = TexCoords.y + ( YSpeed * TimeLapsed);
	return output;
}


vec3 getNormalFromMap()
{
	//normalise it to 0-1
    vec3 tangentNormal = (texture(material.texture_normal, Panner(0.25f, 0.0f)).xyz * 2.0) - 1.0;

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

float UE4SphereMask(vec3 a, vec3 b, float Radius, float Hardness)
{
	float dist = length(a - b);
	float invRadius = 1 / Radius;
	float normDist = dist * invRadius;
	float invHardness = 1 / (1-Hardness);
	float negNormDist = 1 - normDist;
	float unclampedResult = invHardness * negNormDist;
	return clamp(unclampedResult, 0, 1);
	
}

float DistributionGGX(vec3 Norm, vec3 Halfway, float roughness)
{
	//epic double squares the roughness here so i thought i'd try too
	float a2 = (roughness * roughness) *  (roughness * roughness);
	float NDotH = saturate(dot(Norm, Halfway));

	float denom = ((NDotH * NDotH) * (a2 - 1.0f) + 1.0f);
	denom = M_PI * denom * denom;
	return a2 / denom;
}

float GeometrySmith(vec3 Norm, vec3 View, vec3 L, float roughness)
{
    float NdotV = saturate(dot(Norm, View));
    float NdotL = saturate(dot(Norm, L));
    float ggx1 = GeometrySchlickGGX(NdotV, roughness);
    float ggx2 = GeometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
	float r = (roughness + 1.0f);
	float k = (r * r ) / 8.0f;
	float denom = NdotV * (1.0 - k) + k;
	return NdotV / denom;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
	//Dot product result between halfway  and view
	return F0 + (1.0f - F0) * pow(1.0 - cosTheta, 5.0);
}

vec3 ProgrammablePBR(vec3 Norm, vec3 View, vec3 Radiance, vec3 L, LinearMatVals ToParse, float intensity)
{
	vec3 Halfway = normalize(View + L);
	
	//Metelic ratio
	vec3 F0 = vec3(0.04);
	F0 = mix(F0, ToParse.diffuse, ToParse.metallic);

	float NDF = DistributionGGX(Norm, Halfway, ToParse.roughness);
	float GF = GeometrySmith(Norm, View, L, ToParse.roughness);
	vec3 F = fresnelSchlick(saturate(dot(Halfway, View)), F0);
	
	vec3 NGF = NDF * GF * F;
	float denom = 4.0 * saturate(dot(Norm, View)) * saturate(dot(Norm, L)) +  0.001;
	vec3 specular = NGF / denom;
	
	vec3 kS = F;
	vec3 kD = vec3(1.0f) - kS; // energy conservation
	kD *= 1.0f - ToParse.metallic;

	float NdotL = saturate(dot(Norm, L));
	return intensity * ( (kD * ToParse.diffuse / M_PI + specular ) * Radiance * NdotL);
}
LinearMatVals ConvertMapsToPBRValues(Material mats, float Exponent, vec2 texCoords)
{
	float roughness = mix(1, 0.002, dot(vec3(0, 0, 1), getNormalFromMap()));
	float metallic  = 0.5f;// pow(texture(mats.texture_metallic, TexCoords).rgba, vec4(Exponent)).r;
    float ao =  1;//pow(texture(mats.texture_ao, TexCoords).rgba, vec4(2.2)).r;

	vec3 Norm = getNormalFromMap();
	vec3 colourA = vec3(0.02, 0.03, 0.03);
	vec3 colourB = vec3(0.05, 0.07, 0.1);
	float ratio = UE4SphereMask(CamDir, Norm, 1.5f, saturate(dot(CamDir, Normal)) );
	vec3 diffuse = mix (colourA, colourB, ratio);

	return  LinearMatVals(roughness, metallic, ao, diffuse);
}

void main()
{	


	vec3 Norm = getNormalFromMap();
	//Directional Lights
	vec3 r = dirLight.diffuse;
	vec3 L = -dirLight.direction;
	vec3 View = normalize(CamPos - WorldPos);
	LinearMatVals parse = ConvertMapsToPBRValues(material, 1.0f, TexCoords);
	vec3 L0 = ProgrammablePBR(Norm, View, r, L, parse, dirLight.intensity * 10);
	
    color = vec4(parse.diffuse + L0, 1.0f);
	//color = vec4(Normal, 1.0f);
}