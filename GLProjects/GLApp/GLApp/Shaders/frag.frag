#version 330 core
in vec3 ourColour;
out vec4 color;
void main ()
{
color = vec4 (ourColour, 1.0f); 
};
