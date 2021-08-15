#version 430 core

//Source for Hammersley approach 
//http://holger.dammertz.org/stuff/notes_HammersleyOnHemisphere.html

in vec3 Colour;
layout(location = 0) out vec4 FragColour;
layout(location = 1) in vec3 LocalPosition;

const float PI = 3.14159265359;
const vec3 WorldUp = vec3(0.0f, 1.0f, 0.0f);

float RadicalInverse_VanDerCorputBitOperator(uint InBits);
//float RadicalInverse_VanDerCorputNoneOperator(uint InBits);
vec2 Hammersley2d(uint i, uint N);

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

vec3 ImportanceSample(vec2 Xi, vec3 N, float roughness)
{

}

//-------------------------------------------------------------------