//Source for Hammersley approach 
//http://holger.dammertz.org/stuff/notes_HammersleyOnHemisphere.html

#version 430 core

layout(location = 0) out vec4 FragColour;
layout(location = 1) in vec3 LocalPosition;

uniform samplerCube UnConvolutedMap;
const float PI = 3.14159265359;
const vec3 WorldUp = vec3(0.0f, 1.0f, 0.0f);

float RadicalInverse_VanDerCorputBitOperator(uint InBits);
vec2 Hammersley2d(uint i, uint N);
vec3 ImportanceSampleCombineWithGGXNDF(vec2 Xi, vec3 N, float Roughness);

//-------------------------------------------------------------------

void main()
{
    FragColour = vec4(1.0, 1.0, 1.0, 1.0); 
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
    const float R2 = Roughness * Roughness;
    const float Phi = 2 * PI * Xi.x;
    float CosTheta = sqrt((1.0 - Xi.y) / (1.0 + (R2*R2 - 1.0) * Xi.y));
    float SinTheta = sqrt(1.0 - cosTheta*cosTheta);

    vec3 H = vec3(cos(phi) * sinTheta, sin(phi) * sinTheta, cosTheta);

    vec3 Up        = abs(N.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
    vec3 Tangent   = normalize(cross(Up, N));
    vec3 Bitangent = cross(N, Tangent);
	
    vec3 OutputSampleVec = Tangent * H.x + Bitangent * H.y + N * H.z;
    return normalize(OutputSampleVec);
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------