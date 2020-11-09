#version 430 core
#define M_PI 3.14159265359
#define NUMBER_OF_POINT_LIGHTS 1

//-------------------------------------------------------------------

struct DirLight 
{
	float intensity;

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
};

//-------------------------------------------------------------------

in V2F
{
vec3 Normal;
vec2 TexCoords;
vec3 WorldPos;
vec4 FragPosLightSpace;
} fs_in;


out vec4 color;

uniform Material material;
uniform DirLight dirLight;
uniform PointLight pointLights[NUMBER_OF_POINT_LIGHTS];

uniform vec3 CamPos;
uniform vec3 CamDir;
uniform float NearPlane;
uniform float FarPlane;
uniform samplerCube skybox;
uniform sampler2D ShadowMap;
uniform bool ShadowPass = false;


float saturate(float x) {return max(min(x, 1.0f), 0.0f);};
float Desaturate(vec3 InColour);
vec3 GetNormalFromMap();
float DistributionGGX(vec3 Norm, vec3 Halfway, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 Norm, vec3 View, vec3 L, float roughness);
vec3 fresnelSchlick(float cosTheta, vec3 F0, float roughness);
vec3 ProgrammablePBR(vec3 Norm, vec3 View, vec3 Radiance, vec3 L, LinearMatVals ToParse, float intensity);
LinearMatVals ConvertMapsToPBRValues(Material mats, float Exponent, vec2 texCoords);
float ShadowCalculation(vec4 InFragPosLightSpace, vec3 InNormal, vec3 InLightDir);
float LinearizeDepth(float depth);

//-------------------------------------------------------------------

float DistributionGGX(vec3 Norm, vec3 Halfway, float roughness)
{
	//epic double squares the roughness here so i thought i'd try too
	float a2 = (roughness * roughness) *  (roughness * roughness);
	float NDotH = saturate(dot(Norm, Halfway));

	float denom = ((NDotH * NDotH) * (a2 - 1.0f) + 1.0f);
	denom = M_PI * denom * denom;
	return a2 / denom;
}

//-------------------------------------------------------------------

float GeometrySmith(vec3 Norm, vec3 View, vec3 L, float roughness)
{
    float NdotV = saturate(dot(Norm, View));
    float NdotL = saturate(dot(Norm, L));
    float ggx1 = GeometrySchlickGGX(NdotV, roughness);
    float ggx2 = GeometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}

//-------------------------------------------------------------------

float GeometrySchlickGGX(float NdotV, float roughness)
{
	float r = (roughness + 1.0f);
	float k = (r * r ) / 8.0f;
	float denom = NdotV * (1.0 - k) + k;
	return NdotV / denom;
}

//-------------------------------------------------------------------

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
	//Dot product result between halfway  and view
	return F0 + (1.0f - F0) * pow(1.0 - cosTheta, 5.0);
}

//-------------------------------------------------------------------

float CalculateAttenuation(vec3 inFragPos, vec3 inLightPos)
{
	float distance = length(inFragPos - inLightPos);
	return 1.0f / (distance * distance);
}

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

LinearMatVals ConvertMapsToPBRValues(Material mats, float Exponent, vec2 texCoords)
{
	float roughness =  pow(texture(mats.texture_roughness, texCoords).rgba, vec4(Exponent)).r;
	float metallic  =  pow(texture(mats.texture_metallic, texCoords).rgba, vec4(Exponent)).r;
	const float DiffuseExpo = 2.2;
    //float ao =  pow(texture(mats.texture_ao, texCoords).rgba, vec4(DiffuseExpo)).r;
	float ao = Desaturate(pow(texture(mats.texture_normal, texCoords).rgb, vec3(DiffuseExpo)));
	vec3 diffuse = pow(texture(mats.texture_diffuse, texCoords).rgba, vec4(DiffuseExpo)).rgb;
	return  LinearMatVals(roughness, metallic, ao, diffuse);
}

//-------------------------------------------------------------------

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

//-------------------------------------------------------------------

float Desaturate(vec3 InColour)
{
	return (min(InColour.x, min(InColour.y, InColour.z)) + max(InColour.x, max(InColour.y, InColour.z)) * 0.5f);
}

//-------------------------------------------------------------------

// required when using a perspective projection matrix
float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * NearPlane * FarPlane) / (NearPlane + FarPlane - z * (FarPlane - NearPlane));	
}

//-------------------------------------------------------------------

float ShadowCalculation(vec4 InFragPosLightSpace, vec3 InNormal, vec3 InLightDir)
{
	InLightDir = normalize(InLightDir);
    vec3 projCoords = InFragPosLightSpace.xyz / InFragPosLightSpace.w; // perform perspective divide
	projCoords = projCoords * 0.5 + 0.5; // transform to ndc coordinates
	float closestDepth = texture(ShadowMap, projCoords.xy).r;   
	float currentDepth = projCoords.z;  
	
	float bias = max(0.05 * (1.0 - dot(InNormal, InLightDir)), 0.005); 
	float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0; 
	//float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;  

	return shadow;
}

//-------------------------------------------------------------------

void main()
{
	//we normalise this result before returning it
	vec3 Norm = GetNormalFromMap();
	//vec3 Norm = normalize(fs_in.Normal);
	vec3 View = normalize(CamPos - fs_in.WorldPos);

	float RnMPExponent = 1.0f;
	//float RnMPExponent = 2.2;
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

	vec3 L = -dirLight.direction;
	L0 += ProgrammablePBR(Norm, View, r, L, parse, dirLight.intensity);
	
	vec3 ambient = vec3(0.03) * parse.diffuse * parse.ao;
	float Shadow =  ShadowPass? 1.0f - ShadowCalculation(fs_in.FragPosLightSpace, Norm, dirLight.direction) : 1;
    color = vec4(ambient + (L0 * Shadow), 1.0);
	
	//color = vec4(vec3(Shadow));
	//color = color / (color + vec4(1.0));
	//color = pow(color, vec4(1.0/2.2));  
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------