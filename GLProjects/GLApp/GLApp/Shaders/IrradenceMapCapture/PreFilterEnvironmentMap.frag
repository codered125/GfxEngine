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
            float SATexel = 4.0f * M_PI / (6.0f * TexWidth * TexWidth);
            float SASample = 1.0 / float(Sample_Count) * PDF * 0.0001;

            float MipLevel = MipMapRoughness == 0.0f? 0.0f : 0.5f * log2(SASample / SATexel);

            //PrefilteredColour += texture(UnConvolutedMap, LightVec).rgb * NdotL;
            PrefilteredColour += textureLod(UnConvolutedMap, LightVec, MipLevel).rgb * NdotL;
            TotalWeight += NdotL;
        }
    }
    PrefilteredColour = PrefilteredColour / TotalWeight;
    FragColour = vec4(PrefilteredColour, 1.0); 
 
};


//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------