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
uniform vec2 ScreenSize;
const float Radius = 0.5f;
const float Bias = 0.025f;

//-------------------------------------------------------------------

void main()
{  
  const vec2 NoiseScale = vec2(ScreenSize.x / 4.0f, ScreenSize.y / 4.0f);
  const vec3 FragPosition = (View * vec4(texture(PositionalTexture, TexCoords).xyz, 1.0f)).xyz;
  const vec3 FragNormalWorldSpace = normalize(texture(NormalTexture, TexCoords).rgb);
  const vec3 FragNormalViewSpace = mat3(View) * FragNormalWorldSpace;
  const vec3 RandomVec = texture(NoiseTexture, TexCoords * NoiseScale).xyz;    

  const vec3 Tangent   = normalize(RandomVec - FragNormalViewSpace * dot(RandomVec, FragNormalViewSpace));
  const vec3 Bitangent = cross(FragNormalViewSpace, Tangent);
  const mat3 TBN = mat3(Tangent, Bitangent, FragNormalViewSpace); 

  float Occlusion = 0.0f;
  for(int i = 0; i < KernalSize; ++i)
  {
    const vec3 SamplePositionViewSpace = TBN * Samples[i]; 
    const vec3 SamplePosition = FragPosition + SamplePositionViewSpace * Radius;

    const vec4 OffsetViewSpace = vec4(SamplePosition, 1.0f);
    vec4 OffsetClipSpace = Projection * OffsetViewSpace;
    OffsetClipSpace.xyz /= OffsetClipSpace.w; //Perspective divide
    OffsetClipSpace.xyz = OffsetClipSpace.xyz * 0.5 + 0.5; //Remap to 0 - 1

    const float ViewSpaceDepthSample = (View * vec4(texture(PositionalTexture, OffsetClipSpace.xy).xyz, 1.0f)).z;
    const float RangeCheck = smoothstep(0.0, 1.0, Radius / abs(FragPosition.z - ViewSpaceDepthSample));
    Occlusion += (ViewSpaceDepthSample >= SamplePosition.z + Bias ? 1.0 : 0.0) * RangeCheck; 
  }

  FragColour = pow(1.0 - (Occlusion / KernalSize), 7.0f);  
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
