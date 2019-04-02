#version 330 core
#define M_PI 3.14159265359
#define NUMBER_OF_POINT_LIGHTS 3

struct light
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct DirLight 
{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight 
{
    float constant;
    float linear;
    float quadratic;

	vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight
{
    float cutOff;
    float outerCutOff;
    float constant;
    float linear;
    float quadratic;
    
	vec3 position;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

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

in vec2 TexCoords;
in vec3 Normal;
in vec3 WorldPos;

out vec4 color;

uniform Material material;
uniform DirLight dirLight;
uniform SpotLight spotLight;
uniform PointLight pointLights[NUMBER_OF_POINT_LIGHTS];

uniform vec3 CamPos;
bool  blin = false;

float saturate(float x) {return max(min(x, 1.0f), 0.0f);};
vec3 CalcDirLight( DirLight light, vec3 normal, vec3 viewDir );
vec3 getNormalFromMap();
float DistributionGGX(vec3 Norm, vec3 Halfway, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 Norm, vec3 View, vec3 L, float roughness);
vec3 fresnelSchlick(float cosTheta, vec3 F0, float roughness);

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

float CalculateAttenuation(vec3 inFragPos, vec3 inLightPos)
{
	float distance = length(inFragPos - inLightPos);
	return 1.0f / (distance * distance);
}

vec3 getNormalFromMap()
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


void main()
{
	//we normalise this result before returning it
	vec3 Norm = getNormalFromMap();
	vec3 View = normalize(CamPos - WorldPos);

	float roughness =  pow(texture(material.texture_roughness, TexCoords).rgba, vec4(2.2)).r;
	float metallic  =  pow(texture(material.texture_metallic, TexCoords).rgba, vec4(2.2)).r;
    float ao =  pow(texture(material.texture_ao, TexCoords).rgba, vec4(2.2)).r;
	vec3 diffuse = pow(texture(material.texture_diffuse, TexCoords).rgba, vec4(2.2)).rgb;

	//Metelic ratio
	vec3 F0 = vec3(0.04);
	F0 = mix(F0, diffuse, metallic);
	
    // reflectance equation (output)
    vec3 L0 = vec3(0.0);

	for(int i = 0; i < NUMBER_OF_POINT_LIGHTS; i++)	
	{
		//per light radiance
		vec3 L = normalize(pointLights[i].position - WorldPos);
		vec3 Halfway = normalize(View + L);
		vec3 radiance = pointLights[i].diffuse * CalculateAttenuation(WorldPos, pointLights[i].position);

		//Cook-Torrance BRDF (NDF / (4 * dot(w0, n) * dot(wi, n))
		float NormalDistributionFunction = DistributionGGX(Norm, Halfway, roughness);
		float GeometryFunction = GeometrySmith(Norm, View, L, roughness);
		vec3 Fresnel = fresnelSchlick(saturate(dot(Halfway, View)), F0);

		vec3 numerator = NormalDistributionFunction * GeometryFunction * Fresnel;
		float denom = 4.0 * saturate(dot(Norm, View)) * saturate(dot(Norm, L)) +  0.001;
	
		// cant divide by 0
		vec3 specular = numerator / denom;

		vec3 kS = Fresnel;
		vec3 kD = vec3(1.0f) - kS; // energy conservation
		kD *= 1.0f - metallic;

		float NdotL = saturate(dot(Norm, L));
		L0 += 5 * ( (kD * diffuse / M_PI + specular ) * radiance * NdotL);
	}

	vec3 ambient = vec3(0.03) * diffuse * ao;
    color = vec4(ambient + L0, 1.0);

	color = color / (color + vec4(1.0));
    color =  pow(color, vec4(1.0/2.2));  
	
	//color = vec4(pow(texture(material.texture_roughness, TexCoords).rgba, vec4(2.2)).r);
	//color = vec4(getNormalFromMap(), 1.0f);

}
