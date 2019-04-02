#version 330 core
in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;
in vec3 WorldPos;


out vec4 color;

uniform vec3 CamPos;
uniform vec3 CamDir;
uniform float TimeLapsed;
uniform samplerCube skybox;

float saturate(float x) {return max(min(x, 1.0f), 0.0f);};
float UE4SphereMask(vec3 a, vec3 b, float Radius, float Hardness);
vec3 getNormalFromMap();

vec3 getNormalFromMap()
{
	//normalise it to 0-1
    vec3 tangentNormal = (Normal * 2.0) - 1.0;

    vec3 Q1  = dFdx(WorldPos);
    vec3 Q2  = dFdy(WorldPos);
    vec2 st1 = dFdx(TexCoords);
    vec2 st2 = dFdy(TexCoords);

    vec3 N   = normalize(Normal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}

float UE4SphereMask(vec3 a, vec3 b, float Radius, float Hardness)
{
	float dist = length(a - b);
	float invRadius = 1 / Radius;
	float normDist = dist * invRadius;
	float invHardness = 1 / (1-Hardness);
	float negNormDist = 1 - normDist;
	float unclampedResult = invHardness * negNormDist;
	return mix(unclampedResult, 0, 1);
}


void main()
{	
	vec3 colourA = vec3(0.02, 0.03, 0.03);
	vec3 colourB = vec3(0.05, 0.07, 0.1);
	float ratio = UE4SphereMask(CamDir,Normal, 1.5f, saturate(dot(CamDir, Normal))  );
	vec3 output = mix (colourA, colourB, ratio);

    color = vec4( 10 * output, 1.0f);
}