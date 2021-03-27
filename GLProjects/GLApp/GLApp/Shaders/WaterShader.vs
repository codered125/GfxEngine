#version 430 core
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
out vec3 GBitangent;
out vec3 GTangent;
out vec3 FragPos;
out vec2 TexCoords;

uniform mat4 model; //converts local object coords to camera coords
uniform mat4 view; //converts normalised coordinates to window coordinates, aka what your window is
uniform mat4 projection; //converts those camera coordinates to normalised coordinates(0-1)
uniform float TimeLapsed;

vec3 GertsnerWave(vec2 Direction, vec2 worldSpaceXY, float len, float time, float speed, float height, float steepness);
vec3 Equation9Wave(vec2 DDirection, vec2 XYHorizontalPos, float LLength, float TTime, float SSpeed, float AHeight, float QSteepness);
vec3 Equation9WaveNormal(vec2 DDirection, float LLength, float TTime, float SSpeed, float AHeight, float QSteepness, vec3 PPoint);
vec3 Equation9WaveBitangent(vec2 DDirection, float LLength, float TTime, float SSpeed, float AHeight, float QSteepness, vec3 PPoint);
vec3 Equation9WaveTangent(vec2 DDirection, float LLength, float TTime, float SSpeed, float AHeight, float QSteepness, vec3 PPoint);

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

vec3 Equation9Wave(vec2 DDirection, vec2 XYHorizontalPos, float LLength, float TTime, float SSpeed, float AHeight, float QSteepness)
{
	vec3 result;
	float WFrequency = (2  / LLength);
	float PhaseConstant = SSpeed * (2 / LLength);
	float Qi = QSteepness / (WFrequency * AHeight);

	float dDotXY = dot(DDirection, normalize(XYHorizontalPos));
	result.x = (Qi * AHeight * DDirection.x * (cos(WFrequency * dDotXY + (PhaseConstant * TTime))));
	result.z = (Qi * AHeight * DDirection.y * (cos(WFrequency * dDotXY + (PhaseConstant * TTime))));
	result.y = AHeight * (sin(WFrequency * dot(DDirection, XYHorizontalPos) + (PhaseConstant * TTime)));
	return result;
}

//-------------------------------------------------------------------

vec3 Equation9WaveNormal(vec2 DDirection, float LLength, float TTime, float SSpeed, float AHeight, float QSteepness, vec3 PPoint)
{
	vec3 result;
	float WFrequency = (2  / LLength);
	float PhaseConstant = SSpeed * (2 / LLength);
	float Qi = saturate(QSteepness / (WFrequency * AHeight));

	float WA = WFrequency * AHeight;
	vec2 PointXY = normalize(vec2(PPoint.x, PPoint.z));
	float S0 = sin(WFrequency * dot(DDirection, PointXY) + (PhaseConstant * TTime));
	float C0 = cos(WFrequency * dot(DDirection, PointXY) + (PhaseConstant * TTime));

	result.x = DDirection.x *  WA * C0;
	result.x = (result.x + 1) / 2;
	result.x = -result.x;

	result.z = DDirection.y *  WA * C0;
	result.z = (result.z + 1) / 2;
	result.z = -result.z;

	result.y = Qi * WA * S0;
	result.y = (result.y + 1) / 2;	
	result.y = 1 - result.y;
	
	return normalize(result);
}

//-------------------------------------------------------------------

vec3 Equation9WaveBitangent(vec2 DDirection, float LLength, float TTime, float SSpeed, float AHeight, float QSteepness, vec3 PPoint)
{
	vec3 result;
	float WFrequency = (2  / LLength);
	float PhaseConstant = SSpeed * (2 / LLength);
	float Qi = saturate(QSteepness) / (WFrequency * AHeight);

	float WA = WFrequency * AHeight;
	vec3 NormalizedPPoint = normalize(PPoint);
	float S0 = sin(WFrequency * dot(DDirection, vec2(NormalizedPPoint.x, NormalizedPPoint.z)) + (PhaseConstant * TTime));
	float C0 = cos(WFrequency * dot(DDirection, vec2(NormalizedPPoint.x, NormalizedPPoint.z)) + (PhaseConstant * TTime));
	result.x = 1 - (Qi * (DDirection.x * DDirection.x) * WA * S0);
	result.z = -(Qi * DDirection.x * DDirection.y *  WA * S0);
	result.y = (DDirection.x * WA * C0);
	return normalize(result);
}
//-------------------------------------------------------------------

vec3 Equation9WaveTangent(vec2 DDirection, float LLength, float TTime, float SSpeed, float AHeight, float QSteepness, vec3 PPoint)
{
	vec3 result;
	float WFrequency = (2  / LLength);
	float PhaseConstant = SSpeed * (2 / LLength);

	float Qi = saturate(QSteepness) / (WFrequency * AHeight);

	float WA = WFrequency * AHeight;
	float S0 = sin(WFrequency * dot(DDirection, vec2(PPoint.x, PPoint.z)) + (PhaseConstant * TTime));
	float C0 = cos(WFrequency * dot(DDirection, vec2(PPoint.x, PPoint.z)) + (PhaseConstant * TTime));
	result.x = -(Qi * DDirection.x * DDirection.y *  WA * S0);
	result.z = 1 - (Qi * (DDirection.y * DDirection.y) *  WA * S0);
	result.y = 1 - (DDirection.y * WA * C0);
	return normalize(result);
}

//-------------------------------------------------------------------

void main()
{
	const vec2 Dir = vec2(0.0f, 0.0f);
	const vec2 WorldXY = vec2(position.x, position.z);
	const vec2 RelativeDir = normalize(Dir - WorldXY);

	const float WavesLength = 12;
	const float Speed = 4;
	const float Height = 5;
	const float Steepness = 0.5;
	const float Time = TimeLapsed;
	const vec3 Offset = Equation9Wave(RelativeDir, WorldXY, WavesLength, Time, Speed, Height, Steepness);

	FragPos = vec3(model * vec4(position + Offset, 1.0f));
	Normal = mat3(transpose(inverse(model))) * normal;
	GNormal = mat3(transpose(inverse(model))) * Equation9WaveNormal(RelativeDir, WavesLength, Time, Speed, Height, Steepness, FragPos );
	GBitangent = mat3(transpose(inverse(model))) * Equation9WaveBitangent(RelativeDir, WavesLength, Time, Speed, Height, Steepness, FragPos );
	GTangent = mat3(transpose(inverse(model))) * Equation9WaveTangent(RelativeDir, WavesLength, Time, Speed, Height, Steepness, FragPos );
	TexCoords = texCoords;

	gl_Position =  projection * view * model * vec4 (position + Offset, 1.0f);

};

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------