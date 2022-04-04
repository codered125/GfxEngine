#version 430 core
#define KernalSize 64
//-------------------------------------------------------------------

in vec2 TexCoords;
out float FragColour;

uniform sampler2D SSAOTexture;

//-------------------------------------------------------------------

void main()
{  
  const vec2 TexSize = 1.0f / vec2(textureSize(SSAOTexture, 0));
  float Result = 0.0f;
     for (int x = -2; x < 2; ++x) 
    {
        for (int y = -2; y < 2; ++y) 
        {
            vec2 Offset = vec2(float(x), float(y)) * TexSize;
            Result += texture(SSAOTexture, TexCoords + Offset).r;
        }
    }
    FragColour = Result / (4.0 * 4.0);
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------