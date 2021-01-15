#version 330 core
#define M_PI 3.14159265359


//-------------------------------------------------------------------

#include Shaders/HelperShaders/Common.glsl
#include Shaders/HelperShaders/HelperFunctions.glsl

//-------------------------------------------------------------------

layout (location = 0) in vec3 position; 
layout (location = 1) in vec3 normal; 
layout (location = 2) in vec2 texCoords; 

out vec3 Normal;
out vec3 GNormal;
out vec3 FragPos;
out vec2 TexCoords;

uniform mat4 model; //converts local object coords to camera coords
uniform mat4 view; //converts normalised coordinates to window coordinates, aka what your window is
uniform mat4 projection; //converts those camera coordinates to normalised coordinates(0-1)
uniform float TimeLapsed;
uniform vec3 ActorPos;

bool ripple = true;
vec3 Wave(vec2 direct, float inwaves, vec2 inTexCoord, float inTime, float expo, float inheight);
vec3 GertsnerWave(vec2 Direction, vec2 worldSpaceXY, float len, float time, float speed, float height, float steepness);
vec3 GerstnerFunNormal(vec2 Direction, float len, float time, float speed, float height, float steepness, vec3 Point);

//-------------------------------------------------------------------

vec3 Wave(vec2 direct, float inwaves, vec2 inTexCoord, float inTime, float expo, float inheight)
{
	float dirAndWaves = dot(inTexCoord, direct) * inwaves;
	//Because ue4 sine is different to hlsl function
	float applyTime = UE4Sine(dirAndWaves + inTime);
	float normToRange = pow((applyTime +1) / 2 , expo);		
	return vec3(0.0f, 1.0f, 0.0f) * normToRange * inheight;
}

//-------------------------------------------------------------------

vec3 GertsnerWave(vec2 Direction, vec2 worldSpaceXY, float len, float time, float speed, float height, float steepness)
{
	vec3 result;
	float w = ((2 * M_PI) / len);
	float pC = speed * ((2 * M_PI) / len);
	float Qi = steepness / ( w * height);

	result.x = Qi * height * (Direction.x * (UE4Cos(w * dot(Direction, worldSpaceXY) + (pC * time))));
	result.z = Qi * height * (Direction.y * (UE4Cos(w * dot(Direction, worldSpaceXY) + (pC * time))));
	result.y = height * (UE4Sine(w * dot(Direction, worldSpaceXY) + (pC * time)));
	return result;
}

//-------------------------------------------------------------------

vec3 GerstnerFunNormal(vec2 Direction, float len, float time, float speed, float height, float steepness, vec3 Point)
{
	vec3 output;
	float w = ((2 * M_PI) / len );
	float pC = speed * ((2* M_PI) / len);
	//float dDotT = dot(dir, inTexCoord);
	float wa = w * height;
	float Qi = steepness / (w * height);
	float co = UE4Cos(w * dot(Direction, vec2(Point.x, Point.z)) + (pC * time));
	float so = UE4Sine(w * dot(Direction, vec2(Point.x, Point.z)) + (pC * time));
	output.x = -(Direction.x *  wa * co);
	output.z = -(Direction.y *  wa * co);
	output.y = 1 - (Qi * wa * so);
	return normalize(output);
}

//-------------------------------------------------------------------

void main()
{

//float applyRat = saturate(Bilerp(position.y, ActorPos.y, ActorPos.y + 30));
const vec2 Dir = vec2(0.0f, 0.0f);
vec2 WorldXY = vec2(position.x, position.z);
const float Waves = 300;
float Time = TimeLapsed;
const float Speed = 2;
const float Height = 5;
const float Steepness = 0.5;
//vec3 offset = Wave(vec2(1.0f, 0.5f), 2, texCoords, TimeLapsed, 8, 25);
vec3 offset = GertsnerWave(normalize(Dir - WorldXY), WorldXY, Waves, Time, Speed, Height, Steepness);

FragPos = vec3(model * vec4(position + offset, 1.0f));
Normal = mat3(transpose(inverse(model))) * normal;
GNormal = GerstnerFunNormal(Dir, Waves, Time, Speed, Height, Steepness, offset );
TexCoords = texCoords;

gl_Position =  projection * view * model * vec4 (position + offset, 1.0f);
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------