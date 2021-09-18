//Source for Hammersley approach 
//http://holger.dammertz.org/stuff/notes_HammersleyOnHemisphere.html

#version 430 core

#include Shaders/HelperShaders/Common.glsl
#include Shaders/HelperShaders/HelperFunctions.glsl
#include Shaders/HelperShaders/IBLPBR.glsl

in vec2 TexCoords;
out vec2  FragColour;


//-------------------------------------------------------------------

void main()
{   
    float Roughness = TexCoords.y;
    float NdotV = TexCoords.x;
    
    vec3 V = vec3(sqrt(1.0 - NdotV*NdotV), 0.0, NdotV );
    float A = 0.0;
    float B = 0.0; 

    vec3 N = vec3(0.0, 0.0, 1.0);
    
    const uint SAMPLE_COUNT = 1024u;
    for(uint i = 0u; i < SAMPLE_COUNT; ++i)
    {
        // generates a sample vector that's biased towards the
        // preferred alignment direction (importance sampling).
        vec2 Xi = Hammersley2d(i, SAMPLE_COUNT);
        vec3 H = ImportanceSampleCombineWithGGXNDF(Xi, N, Roughness);
        vec3 L = normalize(2.0 * dot(V, H) * H - V);

        float NdotL = saturate(L.z);
        float NdotH = saturate(H.z);
        float VdotH = saturate(dot(V, H));

        if(NdotL > 0.0)
        {
            float G = GeometrySmith(N, V, L, Roughness);
            float G_Vis = (G * VdotH) / (NdotH * NdotV);
            float Fc = pow(1.0 - VdotH, 5.0);

            A += (1.0 - Fc) * G_Vis;
            B += Fc * G_Vis;
        }
    }
    A /= float(SAMPLE_COUNT);
    B /= float(SAMPLE_COUNT);
    FragColour = vec2(A, B); 
 
};


//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------