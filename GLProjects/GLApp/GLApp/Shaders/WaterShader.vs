#version 330 core
layout (location = 0) in vec3 position; 
layout (location = 1) in vec3 normal; 
layout (location = 2) in vec2 texCoords; 

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;
out vec3 WorldPos;


uniform mat4 model; //converts local object coords to camera coords
uniform mat4 view; //converts normalised coordinates to window coordinates, aka what your window is
uniform mat4 projection; //converts those camera coordinates to normalised coordinates(0-1)
uniform float Time;


void main()
{

gl_Position =  projection * view * model * vec4 (position, 1.0f);
FragPos = vec3(model * vec4(position, 1.0f));
Normal = mat3(transpose(inverse(model))) * normal;
WorldPos = FragPos;
TexCoords = texCoords;
};


float UE4Sine(float x)
{
	MaterialFloat result = x * PI * 2;
	result = sin(result);
	return clamp(result, -1.f, 1.f);
}
	
float3 Wave(float2 direct, float inwaves, float2 inTexCoord, float inTime, float expo, float inheight)
{
	const float dirAndWaves = dot(inTexCoord, direct) * inwaves;
	//Because ue4 sine is different to hlsl function
	const float applyTime = UE4Sine(dirAndWaves + inTime);
	const float normToRange = pow((applyTime +1) / 2 , expo);		
	return MaterialFloat3(0.0f, 0.0f, 1.0f) * normToRange * inheight;
}