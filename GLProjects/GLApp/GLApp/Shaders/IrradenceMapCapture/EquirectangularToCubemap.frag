#version 430 core

in vec3 Colour;
layout(location = 0) out vec4 FragColour;
layout(location = 1) in vec3 LocalPosition;

uniform sampler2D EquirectangularMap;

//Reciprocals of PI
//0.1591 = 1/6.28319(=>2PI) = 360 degrees in radians //0.3183 = 1/3.14159(=>PI) = 180 degrees in radians
const vec2 InvAtan = vec2(0.1591, 0.3183);

vec2 SampleSphericalMap( vec3 InVec)
{
	vec2 uv = vec2(atan(InVec.z, InVec.x), asin(InVec.y));
	uv*= InvAtan;
	uv+= 0.5f;
	return uv;
}
void main ()
{

	vec2 uv = SampleSphericalMap(normalize(LocalPosition));
	vec3 Colour = texture(EquirectangularMap, uv).rgb;
	FragColour = vec4(Colour, 1.0f);
			

};
