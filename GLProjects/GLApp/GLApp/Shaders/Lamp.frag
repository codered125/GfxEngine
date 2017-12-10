#version 330 core
in vec3 Colour;
in float outTime;
out vec4 color;

void main ()
{
vec3 zero = vec3(0.0f, 0.0f, 0.0f);
vec3 Result = zero + ((Colour - zero) * outTime);
color = vec4(Result, 1.0f);
};
