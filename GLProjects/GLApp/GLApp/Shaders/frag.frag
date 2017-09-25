#version 330 core
in vec3 ourColour;
in vec2 TexCoord;

out vec4 color;
uniform sampler2D ourTexture1;
void main ()
{
color = texture(ourTexture1, TexCoord);  //vec4 (ourColour, 1.0f); 
};
