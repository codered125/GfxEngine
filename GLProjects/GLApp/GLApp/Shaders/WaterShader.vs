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
float saturate(float x) {return max(min(x, 1.0f), 0.0f);};
float Bilerp (float min, float max, float T);

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

float Bilerp (float min, float max, float T)
{
	return	(T - min) / (max - min);
}


void main()
{
	FragPos = vec3(model * vec4(position, 1.0f));
	Normal = mat3(transpose(inverse(model))) * normal;
	TexCoords = texCoords;
	WorldPos = FragPos;
	float topside = saturate(Bilerp(ActorPos.z, ActorPos.z + 2.0f, position.z));
	
	vec3 Output = topside * Wave(vec2(1.0f, 0.5f), 2, texCoords, TimeLapsed, 8, 25);

	gl_Position =  projection * view * model * vec4 (position + Output, 1.0f);
};