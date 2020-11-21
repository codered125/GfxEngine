//-------------------------------------------------------------------

#include Shaders/HelperShaders/Common.glsl

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

float GeometrySchlickGGX(float NdotV, float roughness)
{
	float r = (roughness + 1.0f);
	float k = (r * r ) / 8.0f;
	float denom = NdotV * (1.0 - k) + k;
	return NdotV / denom;
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

LinearMatVals ConvertMapsToPBRValues(Material mats, float Exponent, vec2 texCoords)
{
	float roughness =  pow(texture(mats.texture_roughness, texCoords).rgba, vec4(Exponent)).r;
	float metallic  =  pow(texture(mats.texture_metallic, texCoords).rgba, vec4(Exponent)).r;
	
	const float DiffuseExpo = 2.2;
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
	return intensity * ((kD * ToParse.diffuse / M_PI + specular ) * Radiance * NdotL);
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------