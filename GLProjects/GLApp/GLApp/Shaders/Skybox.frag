#version 330 core
in vec3 TexCoords;
out vec4 color;

uniform samplerCube skybox;

void main()
{
    //color = textureLod(skybox, TexCoords, 4);
    color = texture(skybox, TexCoords);
}
