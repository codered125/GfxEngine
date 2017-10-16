#version 330 core

out vec4 color;

uniform vec3 objectColour;
uniform vec3 lightColour;

void main ()
{
color = vec4(lightColour  * objectColour, 1.0f);
};
