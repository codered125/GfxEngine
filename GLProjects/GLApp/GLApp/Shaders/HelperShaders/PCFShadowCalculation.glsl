
//-------------------------------------------------------------------

float DetermineShadow(vec4 InFragPosLightSpace, vec3 InNormal, vec3 InLightDir, sampler2D ShadowMap)
{
  //perform perspective divide, not necassary on orthagraphic but is a safe bet for both
 	vec3 ProjCoords = InFragPosLightSpace.xyz / InFragPosLightSpace.w; 
 	ProjCoords = ProjCoords * 0.5 + 0.5; // transform to ndc coordinates

	float ClosestDepth = texture(ShadowMap, ProjCoords.xy).r;   
	float CurrentDepth = ProjCoords.z;  
	float Bias = max(0.05 * (1.0 - dot(InNormal, InLightDir)), 0.005); 

  //check whether current frag pos is in shadow
  //PCF
  float Shadow = 0.0;
  vec2 TexelSize = 1.0 / textureSize(ShadowMap, 0);
  int Sample = 4;
  for(int x = -Sample; x <= Sample; ++x)
  {
  	for(int y = -Sample; y <= Sample; ++y)
    {
      float PCfDepth = texture(ShadowMap, ProjCoords.xy + vec2(x, y) * TexelSize).r; 
      Shadow += CurrentDepth - Bias > PCfDepth? 1.0 : 0.0;        
    }   
 	}
  Shadow /= pow((Sample * 2) + 1, 2);
 //Shadow = CurrentDepth - Bias > ClosestDepth? 1.0 : 0.0;
 //keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
 if(ProjCoords.z > 1.0)
 {
	 Shadow = 0.0;
 }

	return Shadow;
}

//-------------------------------------------------------------------