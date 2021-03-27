#version 430 core
#define NUMBER_OF_POINT_LIGHTS 3

//-------------------------------------------------------------------

#include Shaders/HelperShaders/Common.glsl
#include Shaders/HelperShaders/HelperFunctions.glsl
#include Shaders/HelperShaders/PCFShadowCalculation.glsl
#include Shaders/HelperShaders/PBR.glsl

//-------------------------------------------------------------------

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;
in vec3 GBitangent;
in vec3 GTangent;
in vec3 GNormal;

out vec4 color;

uniform vec3 CamPos;
uniform vec3 CamDir;
uniform float TimeLapsed;
uniform Material material;
uniform DirLight dirLight;
vec3 Equation9WaveNormal(vec2 DDirection, float LLength, float TTime, float SSpeed, float AHeight, float QSteepness, vec3 PPoint);
vec3 GetNormalFromMap();

//-------------------------------------------------------------------

vec3 GetNormalFromMap()
{
	//normalise it to 0-1
	//mat3 TBN = mat3(normalize(GTangent), normalize(GBitangent),normalize( GNormal));
	//normalise it to 0-1
    vec3 tangentNormal = (GNormal * 2.0) - 1.0;

    vec3 Q1  = dFdx(FragPos);
    vec3 Q2  = dFdy(FragPos);
    vec2 st1 = dFdx(TexCoords);
    vec2 st2 = dFdy(TexCoords);

    vec3 N   = normalize(Normal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

	vec3 result = TBN * tangentNormal;
    return normalize(vec3(result.y, result.x, result.z));
   //return normalize(GNormal);
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

	result.x = -(DDirection.x *  WA * C0);
	result.z = -(DDirection.y *  WA * C0);
	result.y = 1 - (Qi * WA * S0);
	return normalize(result);
}

//-------------------------------------------------------------------

void main()
{	
	vec3 Norm = GetNormalFromMap();
	//vec3 Norm = Normal;

	//Directional Lights
	float instense = dirLight.intensity;
	vec3 r = dirLight.diffuse;
	vec3 L = -dirLight.direction;
	vec3 View = normalize(CamPos - FragPos);

	LinearMatVals parse;

	float MaskHardness = saturate(dot(normalize(View), Norm));
	float SphereMask = UE4SphereMask(normalize(View), Norm, 5, MaskHardness);

	vec3 CloserColour = vec3(0.023, 0.034, 0.036);
 	vec3 FurtherColour = vec3(0.052, 0.07, 0.11);
	//vec3 CloserColour = normalize(vec3(0.023, 0.034, 0.036));
 	//vec3 FurtherColour = normalize(vec3(0.549, 0.5843, 0.8549) );
	parse.diffuse = mix(CloserColour, FurtherColour, pow(SphereMask, 0.03)) * 2;

	float RRatio = saturate(dot(vec3(0.0f, 1.0f, 0.0f), Norm));
	parse.roughness = mix(0.50f, 0.002f, RRatio);
	
	
	parse.ao = 1.0f;
	parse.metallic = 0.10f;

	vec3 L0 = ProgrammablePBR(Norm, View, r, L, parse, instense);
	//float roughRat = saturate(Bilerp(WorldPos.y, ActorPos.y, ActorPos.y + 50));
	vec3 ambient = vec3(0.03) * parse.diffuse * parse.ao;
    color = vec4( L0 , 1.0f);
	color = vec4(Norm, 1.0f);
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------