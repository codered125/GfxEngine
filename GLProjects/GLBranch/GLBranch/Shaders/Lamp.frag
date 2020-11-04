#version 330 core

in vec3 Colour;
layout(location = 0) out vec4 FragColour;
layout(location = 1) out vec4 BrightColour;

uniform float Time;
uniform float TimeLapsed;
uniform mat4 model; //converts local object coords to camera coords
uniform mat4 view; //converts normalised coordinates to window coordinates, aka what your window is
uniform mat4 projection; //converts those camera coordinates to normalised coordinates(0-1)
uniform vec3 inColour;

float exposure = 1.5;
const float offset = 1 /300.0f;

void main ()
{

	vec3 hdrColor = Colour * 100;
	vec3 result = vec3(1.0) - exp(-hdrColor * exposure); // exposure
	FragColour = vec4(result, 1.0f);
			

};
