#version 430 core

in vec3 Colour;
layout(location = 0) out vec4 FragColour;
layout(location = 1) in vec3 LocalPosition;

uniform samplerCube UnConvolutedMap;
const float PI = 3.14159265359;
const vec3 WorldUp = vec3(0.0f, 1.0f, 0.0f);

void main()
{
	const vec3 N = normalize(LocalPosition);
	vec3 I = vec3(0.0f);

	const vec3 LocalRight = normalize(cross(WorldUp, N));
	const vec3 LocalUp = normalize(cross(N, LocalRight));
	const float SampleDelta = 0.025f;
	int NumberOfSamples = 0;

	for(float Phi = 0.0; Phi < 2.0 * PI; Phi += SampleDelta)
    {
        for(float Theta = 0.0; Theta < 0.5 * PI; Theta += SampleDelta)
        {
            // spherical to cartesian
            vec3 TangentSToC = vec3(sin(Theta) * cos(Phi),  sin(Theta) * sin(Phi), cos(Theta));
            // tangent space to world
            vec3 TSpaceToWorldSpace = TangentSToC.x * LocalRight + TangentSToC.y * LocalUp + TangentSToC.z * N; 

            I += texture(UnConvolutedMap, TSpaceToWorldSpace).rgb * cos(Theta) * sin(Theta);
            NumberOfSamples++;
        }
    }
    I = PI * I * (1.0 / float(NumberOfSamples));
    
    FragColour = vec4(I, 1.0); 


};
