//Source for Hammersley approach 
//http://holger.dammertz.org/stuff/notes_HammersleyOnHemisphere.html

#version 430 core

#include Shaders/HelperShaders/Common.glsl
#include Shaders/HelperShaders/HelperFunctions.glsl
#include Shaders/HelperShaders/IBLPBR.glsl

layout(location = 1) in vec3 LocalPosition;
out vec4 FragColour;

uniform samplerCube UnConvolutedMap;
uniform float MipMapRoughness;
const float PI = 3.14159265359;

float RadicalInverse_VanDerCorputBitOperator(uint InBits);
vec2 Hammersley2d(uint i, uint N);
vec3 ImportanceSampleCombineWithGGXNDF(vec2 Xi, vec3 N, float Roughness);

//-------------------------------------------------------------------

void main()
{
    const vec3 N = normalize(LocalPosition);
    const vec3 R = N;
    const vec3 V = N;

    const uint Sample_Count = 1024u;
    float TotalWeight = 0;
    vec3 PrefilteredColour;

    for(uint i = 0u; i < Sample_Count; ++i)
    {
        vec2 Xi = Hammersley2d(uint(i), uint(Sample_Count));
        vec3 Halfway = ImportanceSampleCombineWithGGXNDF(Xi, N, MipMapRoughness);
        vec3 LightVec = normalize(2.0 * dot(V, Halfway) * Halfway - V);

        float NdotL = saturate(dot(N, LightVec));
        float NdotH = saturate(dot(N, Halfway));
        float HdotV = saturate(dot(Halfway, V));
        if(NdotL > 0.0f)
        {
            float D  = DistributionGGX(N, Halfway, MipMapRoughness);
            float PDF = (D * NdotH / (4.0 * HdotV)) + 0.0001; 

            float TexWidth = 512;
            float SATexel = 4.0f * PI / (6.0f * TexWidth * TexWidth);
            float SaSample = 1.0 / float(Sample_Count) * PDF * 0.0001;

            float MipLevel = MipMapRoughness == 0.0f? 0.0f : 0.5f * log2(SaSample / SATexel);

            //PrefilteredColour += texture(UnConvolutedMap, LightVec).rgb * NdotL;
            PrefilteredColour += textureLod(UnConvolutedMap, LightVec, MipLevel).rgb * NdotL;
            TotalWeight += NdotL;
        }
    }
    PrefilteredColour = PrefilteredColour / TotalWeight;
    FragColour = vec4(PrefilteredColour, 1.0); 
 
};

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
    const float Phi = 2 * PI * Xi.x;
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
//-------------------------------------------------------------------
//-------------------------------------------------------------------