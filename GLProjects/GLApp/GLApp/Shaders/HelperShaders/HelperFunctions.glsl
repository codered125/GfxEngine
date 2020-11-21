
//-------------------------------------------------------------------

float Desaturate(vec3 InColour)
{
  return (min(InColour.x, min(InColour.y, InColour.z)) + max(InColour.x, max(InColour.y, InColour.z)) * 0.5f);
}

//-------------------------------------------------------------------

float saturate(float x) 
{
  return max(min(x, 1.0f), 0.0f);
}

//-------------------------------------------------------------------

float UE4SphereMask(vec3 a, vec3 b, float Radius, float Hardness)
{
	float dist = length(a - b);
	float invRadius = 1 / Radius;
	float normDist = dist * invRadius;
	float invHardness = 1 / (1-Hardness);
	float negNormDist = 1 - normDist;
	float unclampedResult = invHardness * negNormDist;
	return saturate(unclampedResult); 
}

//-------------------------------------------------------------------

vec2 Panner(float XSpeed, float YSpeed, vec2 InTexCoords, float TimeLapsed)
{
	vec2 OutCoords;
	OutCoords.x = InTexCoords.x + ( XSpeed * TimeLapsed);
	OutCoords.y = InTexCoords.y + ( YSpeed * TimeLapsed);
	return OutCoords;
}

//-------------------------------------------------------------------

float Bilerp(float T, float min, float max)
{
	return saturate((T - min) / (max - min));
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------