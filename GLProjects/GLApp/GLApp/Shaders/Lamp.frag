#version 330 core
in vec3 Colour;
out vec4 color;

void main ()
{
color = vec4(Colour, 1.0f);
};
