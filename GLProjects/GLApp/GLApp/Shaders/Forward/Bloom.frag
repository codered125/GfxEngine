#version 430 core

in vec2 TexCoords;

layout (location = 0) out vec4 FragColour;
layout (location = 1) out vec4 BrightColour; 
layout (location = 2) uniform sampler2D ScreenTexture;

void main()
{ 	
	vec3 HdrColor = texture(screenTexture, TexCoords).rgb;

    // If bright enough save colour otherwise discard it
    float Brightness = dot(HdrColor.rgb, vec3(0.2126, 0.7152, 0.0722));
  	BrightColour = Brightness > 1.0 ? vec4(HdrColor.rgb, 1.0) : vec4(0.0, 0.0, 0.0, 1.0);

}
