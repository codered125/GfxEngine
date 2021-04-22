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

	const vec3 TangentRight = normalize(cross(WorldUp, N));
	const vec3 TangentUp = normalize(cross(N, TangentRight));
	const float SampleDelta = 0.025f;
	int NumberOfSamples = 0;

	for(float Phi = 0.0; Phi < 2.0 * PI; Phi += SampleDelta)
    {
        for(float theta = 0.0; theta < 0.5 * PI; theta += SampleDelta)
        {
            // spherical to cartesian (in tangent space)
            vec3 tangentSample = vec3(sin(theta) * cos(Phi),  sin(theta) * sin(Phi), cos(theta));
            // tangent space to world
            vec3 sampleVec = tangentSample.x * TangentRight + tangentSample.y * TangentUp + tangentSample.z * N; 

            I += texture(UnConvolutedMap, sampleVec).rgb * cos(theta) * sin(theta);
            NumberOfSamples++;
        }
    }
    I = PI * I * (1.0 / float(NumberOfSamples));
    
    FragColour = vec4(I, 1.0); 


};
