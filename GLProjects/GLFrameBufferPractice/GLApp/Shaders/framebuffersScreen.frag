#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;

void main()
{ 
	//inverted
	vec3 PreProssed = vec3 (texture(screenTexture, TexCoords));

    FragColor = vec4(PreProssed, 1.0f);
}