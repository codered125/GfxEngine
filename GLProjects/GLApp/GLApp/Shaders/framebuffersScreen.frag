#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform bool hdr;
uniform float exposure;

void main()
{ 
//const float gamma = 2.2;
const float gamma = 1.0f;
    vec3 hdrColor = texture(screenTexture, TexCoords).rgb;
    if(hdr)
    {
        //vec3 result = hdrColor / (hdrColor + vec3(1.0)); // reinhard
		vec3 result = vec3(1.0) - exp(-hdrColor * exposure); // exposure
        //also gamma correct while we're at it       
        result = pow(result, vec3(1.0 / gamma));
        FragColor = vec4( result, 1.0);
    }
    else
    {
        vec3 result = pow(hdrColor, vec3(1.0 / gamma));
        FragColor = vec4(result, 1.0);
    }
}