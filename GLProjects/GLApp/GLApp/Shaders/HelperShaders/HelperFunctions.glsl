
//-------------------------------------------------------------------

float Desaturate(vec3 InColour)
{
  return (min(InColour.x, min(InColour.y, InColour.z)) + max(InColour.x, max(InColour.y, InColour.z)) * 0.5f);
}

//-------------------------------------------------------------------

float saturate(float x) 
{
  return max(min(x, 1.0f), 0.0f);
};

//-------------------------------------------------------------------