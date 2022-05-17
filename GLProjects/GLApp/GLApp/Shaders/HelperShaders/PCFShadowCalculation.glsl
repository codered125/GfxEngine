
//-------------------------------------------------------------------

float DetermineShadow(vec4 InFragPosLightSpace, vec3 InNormal, vec3 InLightDir, sampler2D InShadowMap)
{
  //perform perspective divide, not necassary on orthagraphic but is a safe bet for both
 	vec3 ProjCoords = InFragPosLightSpace.xyz / InFragPosLightSpace.w; 
 	ProjCoords = ProjCoords * 0.5 + 0.5; // transform to ndc coordinates

	float CurrentDepth = ProjCoords.z;  
  float Bias = max(0.001 * (1.0 - dot(InNormal, InLightDir)), 0.001); 

  //check whether current frag pos is in shadow
  //PCF
  float Shadow = 0.0;
  const vec2 TexelSize = 1.0 / textureSize(InShadowMap, 0);
  const int Sample = 4;
  for(int x = -Sample; x <= Sample; ++x)
  {
  	for(int y = -Sample; y <= Sample; ++y)
    {
      float PCfDepth = texture(InShadowMap, ProjCoords.xy + vec2(x, y) * TexelSize).r; 
      Shadow += CurrentDepth - Bias > PCfDepth? 1.0 : 0.0;        
    }   
 	}

  Shadow /= pow((Sample * 2) + 1, 2);
  //keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
  if(ProjCoords.z > 1.0)
  {
	   Shadow = 0.0;
  }
	return Shadow;
}

//-------------------------------------------------------------------

int GetCascadingLayer(vec4 InFragPosWorldSpace, mat4 InView, float InCascadingLevelsArray[10], int InCascadeCount)
{
  const vec4 FragPosViewSpace = InView * InFragPosWorldSpace;
  const float DepthValue = abs(FragPosViewSpace.z);
    
  int Layer = -1;
  for (int i = 0; i < InCascadeCount; ++i)
  {
    if (DepthValue < InCascadingLevelsArray[i])
    {
      Layer = i;
      break;
    }
  }

  if (Layer == -1)
  {
    Layer = InCascadeCount;
  }  
  return Layer;
}

//-------------------------------------------------------------------

float DetermineShadowCSM(vec4 InFragPosLightSpace, vec3 InNormal, vec3 InLightDir, sampler2DArray ShadowMaps, int Layer, float InCascadingLevelsArray[10], int InCascadeCount, float InFarPlane)
{
  //perform perspective divide, not necassary on orthagraphic but is a safe bet for both
 	vec3 ProjCoords = InFragPosLightSpace.xyz / InFragPosLightSpace.w; 
 	ProjCoords = ProjCoords * 0.5 + 0.5; // transform to ndc coordinates

	float CurrentDepth = ProjCoords.z;  
  if(CurrentDepth > 1.0f )
  {
    return 0.0f; 
  }
  
  float Bias = max(0.001 * (1.0 - dot(InNormal, InLightDir)), 0.001);
  //float Bias = max(0.05 * (1.0 - dot(InNormal, InLightDir)), 0.005); 
  if (Layer == InCascadeCount)
  {
    Bias *= 1 / (InFarPlane * 0.5f);
  }
  else
  {
    Bias *= 1 / (InCascadingLevelsArray[Layer] * 0.5f);
  }

  //check whether current frag pos is in shadowp
  //PCF
  float Shadow = 0.0;
  vec2 TexelSize = 1.0 / vec2(textureSize(ShadowMaps, 0));
  int Sample = 4;
  for(int x = -Sample; x <= Sample; ++x)
  {
  	for(int y = -Sample; y <= Sample; ++y)
    {
      float PCfDepth = texture(ShadowMaps, vec3(ProjCoords.xy + vec2(x, y) * TexelSize, Layer)).r;
      Shadow += (CurrentDepth - Bias) > PCfDepth? 1.0 : 0.0;        
    }   
 	}
  Shadow /= pow((Sample * 2) + 1, 2);
  //keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
  return Shadow;
}

//-------------------------------------------------------------------