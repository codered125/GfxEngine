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

float GeometrySchlickGGXIBL(float NdotV, float roughness)
{
	float a = roughness * roughness;
	float k = a / 2.0f;
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

// ----------------------------------------------------------------------------

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}  

//-------------------------------------------------------------------

float CalculateAttenuation(vec3 inFragPos, vec3 inLightPos)
{
	float distance = length(inFragPos - inLightPos);
	return 1.0f / (distance * distance);
}

//-------------------------------------------------------------------

float RadicalInverse_VanDerCorputBitOperator(uint InBits)
{
    InBits = (InBits << 16u) | (InBits >> 16u);
    InBits = ((InBits & 0x55555555u) << 1u) | ((InBits & 0xAAAAAAAAu) >> 1u);
    InBits = ((InBits & 0x33333333u) << 2u) | ((InBits & 0xCCCCCCCCu) >> 2u);
    InBits = ((InBits & 0x0F0F0F0Fu) << 4u) | ((InBits & 0xF0F0F0F0u) >> 4u);
    InBits = ((InBits & 0x00FF00FFu) << 8u) | ((InBits & 0xFF00FF00u) >> 8u);
    return float(InBits) * 2.3283064365386963e-10; // / 0x100000000
};

//-------------------------------------------------------------------

vec2 Hammersley2d(uint i, uint N)
{
    return vec2(float(i)/float(N), RadicalInverse_VanDerCorputBitOperator(i));
};

//-------------------------------------------------------------------

vec3 ImportanceSampleCombineWithGGXNDF(vec2 Xi, vec3 N, float Roughness)
{
    //Epic square it from disneys PBR research
    const float R2 = Roughness * Roughness;
    const float Phi = 2 * M_PI * Xi.x;
    const float CosTheta = sqrt((1.0 - Xi.y) / (1.0 + (R2*R2 - 1.0) * Xi.y));
    const float SinTheta = sqrt(1.0 - CosTheta*CosTheta);

    const vec3 H = vec3(cos(Phi) * SinTheta, sin(Phi) * SinTheta, CosTheta);

    const vec3 Up        = abs(N.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
    const vec3 Tangent   = normalize(cross(Up, N));
    const vec3 Bitangent = cross(N, Tangent);
	
    vec3 OutputSampleVec = Tangent * H.x + Bitangent * H.y + N * H.z;
    return normalize(OutputSampleVec);
}

//-------------------------------------------------------------------

LinearMatVals ConvertMapsToPBRValues(Material mats, float Exponent, vec2 texCoords)
{
	float roughness =  pow(texture(mats.texture_roughness, texCoords).rgba, vec4(Exponent)).r;
	float metallic  =  pow(texture(mats.texture_metallic, texCoords).rgba, vec4(Exponent)).r;
	float alpha = texture(mats.texture_diffuse, texCoords).a;
	
	//const float DiffuseExpo = 1.0;
	 const float DiffuseExpo = 2.2;
	//float ao = Desaturate(pow(texture(mats.texture_normal, texCoords).rgb, vec3(DiffuseExpo)));
	float ao = 1.0f;
	vec3 diffuse = pow(texture(mats.texture_diffuse, texCoords).rgba, vec4(DiffuseExpo)).rgb;
	return  LinearMatVals(roughness, metallic, ao, diffuse, alpha);
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

vec3 IBLAmbientSpecular(vec3 Norm, vec3 View, LinearMatVals ToParse, sampler2D BrdfLUT, samplerCube IrradenceMap, samplerCube PrefiltMap)
{
	vec3 R = reflect(-View, Norm); 

	//Metelic ratio
	vec3 F0 = vec3(0.04);
	F0 = mix(F0, ToParse.diffuse, ToParse.metallic);
    vec3 F = fresnelSchlickRoughness(saturate(dot(Norm, View)), F0, ToParse.roughness);
    
    // sample both the pre-filter map and the BRDF lut and combine them together as per the Split-Sum approximation to get the IBL specular part.
    const float MAX_REFLECTION_LOD = 4.0;
    vec3 prefilteredColor = textureLod(PrefiltMap, R,  ToParse.roughness * MAX_REFLECTION_LOD).rgb;    
    vec2 brdf  = texture(BrdfLUT, vec2(saturate(dot(Norm, View)), ToParse.roughness)).rg;
    vec3 specular = prefilteredColor * (F * brdf.x + brdf.y);
	return specular;
}


//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------