#version 330 core
#define M_PI 3.14159265359

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
uniform float TimeLapsed;
uniform vec3 ActorPos;


bool ripple = true;
vec3 Wave(vec2 direct, float inwaves, vec2 inTexCoord, float inTime, float expo, float inheight);
float UE4Sine(float x);
float Bilerp(float T, float min, float max);
float saturate(float x) {return max(min(x, 1.0f), 0.0f);};

float Bilerp(float T, float min, float max)
{
	return (T - min) / (max - min);
}

float UE4Sine(float x)
{
	float result = x * M_PI * 2;
	result = sin(result);
	return clamp(result, -1.f, 1.f);
}
	
vec3 Wave(vec2 direct, float inwaves, vec2 inTexCoord, float inTime, float expo, float inheight)
{
	float dirAndWaves = dot(inTexCoord, direct) * inwaves;
	//Because ue4 sine is different to hlsl function
	float applyTime = UE4Sine(dirAndWaves + inTime);
	float normToRange = pow((applyTime +1) / 2 , expo);		
	return vec3(0.0f, 0.0f, 1.0f) * normToRange * inheight;
}


void main()
{
FragPos = vec3(model * vec4(position, 1.0f));
Normal = mat3(transpose(inverse(model))) * normal;
TexCoords = texCoords;
WorldPos = FragPos;

float applyRat = saturate(Bilerp(position.z, ActorPos.z, ActorPos.z + 5));
vec3 offset = Wave(vec2(0.6f, 0.4f), 4, texCoords, TimeLapsed, 8, 50);

gl_Position =  projection * view * model * vec4 (position + offset, 1.0f);

};
