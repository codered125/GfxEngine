#version 430 core
#define KernalSize 64
//-------------------------------------------------------------------

in vec2 TexCoords;
out float FragColour;

uniform sampler2D PositionalTexture;
uniform sampler2D NormalTexture;
uniform sampler2D NoiseTexture;

uniform vec3 Samples[KernalSize];
uniform mat4 Projection;
uniform mat4 View;

const vec2 NoiseScale = vec2(2560.0f / 4.0f, 1377.0f / 4.0f);
//const float Radius = 2.5f;
const float Radius = 0.5f;
//const float Bias = 0.0025f;
const float Bias = 0.025f;

//-------------------------------------------------------------------

void main()
{  
  const vec3 FragPos = vec3(View * vec4(texture(PositionalTexture, TexCoords).xyz, 1.0f));
  const vec3 FragNormalWorldSpace = texture(NormalTexture, TexCoords).rgb;

  // discard fragment if normal is empty
  if (FragNormalWorldSpace == vec3(0.0f)) discard;

  const vec3 FragNormalViewSpace = normalize(mat3(View) * FragNormalWorldSpace);
  //const vec3 FragNormalViewSpace = FragNormal;

  const vec3 RandomVec = texture(NoiseTexture, TexCoords * NoiseScale).xyz;    
  float Occlusion = 0.0f;
  for(int i = 0; i < KernalSize; ++i)
  {
    const vec3 SamplePositionViewSpace = vec3(View * vec4(Samples[i], 1.0f)); //From tangent space
    const vec3 SamplePosition = FragPos + SamplePositionViewSpace * Radius;

    const vec4 OffsetViewSpace = vec4(SamplePosition, 1.0f);
    vec4 OffsetClipSpace = Projection * OffsetViewSpace;
    OffsetClipSpace.xyz /= OffsetClipSpace.w; // Perspective divide
    OffsetClipSpace.xyz = OffsetClipSpace.xyz * 0.5 + 0.5; //Remap to 0 - 1
    
    const float SamplesDepthViewSpace = vec3(View * texture(PositionalTexture, OffsetClipSpace.xy)).z;
    float RangeCheck = smoothstep(0.0, 1.0, Radius / abs(FragPos.z - SamplesDepthViewSpace));
    Occlusion += (SamplesDepthViewSpace >= SamplePosition.z + Bias ? 1.0 : 0.0) * RangeCheck; 
  }

  FragColour = 1.0 - (Occlusion / KernalSize);  
  FragColour = Occlusion;
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------