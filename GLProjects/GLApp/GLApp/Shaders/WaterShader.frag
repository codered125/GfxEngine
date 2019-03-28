#version 330 core
in vec3 TexCoords;
in vec3 FragPos;
in vec3 Normal;
in vec3 WorldPos;


out vec4 color;

uniform float Time;
uniform samplerCube skybox;

void main()
{
    color = texture(skybox, TexCoords);
}


