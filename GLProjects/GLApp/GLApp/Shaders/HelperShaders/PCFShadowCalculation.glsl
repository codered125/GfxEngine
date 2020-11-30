
//-------------------------------------------------------------------

float DetermineShadow(vec4 InFragPosLightSpace, vec3 InNormal, vec3 InLightDir, sampler2D ShadowMap)
{
  // perform perspective divide, not necassary on orthagraphic but is a safe bet for both
 	vec3 ProjCoords = InFragPosLightSpace.xyz / InFragPosLightSpace.w; 
	ProjCoords = ProjCoords * 0.5 + 0.5; // transform to ndc coordinates

	float ClosestDepth = texture(ShadowMap, ProjCoords.xy).r;   
	float CurrentDepth = ProjCoords.z;  

	float Bias = max(0.05 * (1.0 - dot(InNormal, InLightDir)), 0.005); 

  // check whether current frag pos is in shadow
  //PCF
  float Shadow = 0.0;
  vec2 TexelSize = 1.0 / textureSize(ShadowMap, 0);
  for(int x = -3; x <= 3; ++x)
  {
  	for(int y = -3; y <= 3; ++y)
    {
      float PcfDepth = texture(ShadowMap, ProjCoords.xy + vec2(x, y) * TexelSize).r; 
      Shadow += CurrentDepth - Bias > PcfDepth  ? 1.0 : 0.0;        
    }   
 	}
  Shadow /= 49.0;
    
  //keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
  if(ProjCoords.z > 1.0)
  {
	  Shadow = 0.0;
  }

	return Shadow;
}

//-------------------------------------------------------------------