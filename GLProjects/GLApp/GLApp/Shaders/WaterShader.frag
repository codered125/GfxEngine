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
in vec3 GNormal;

out vec4 color;

uniform vec3 CamPos;
uniform vec3 CamDir;
uniform float TimeLapsed;
uniform Material material;
uniform vec3 ActorPos;
uniform DirLight dirLight;

vec3 GetNormalFromMap();

//-------------------------------------------------------------------

vec3 GetNormalFromMap()
{
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

    return normalize(TBN * tangentNormal);
}

//-------------------------------------------------------------------

void main()
{	
	vec3 Norm = GetNormalFromMap();

	//Directional Lights
	float instense = dirLight.intensity;
	vec3 r = dirLight.diffuse;
	vec3 L = -dirLight.direction;
	vec3 View = normalize(CamPos - FragPos);

	LinearMatVals parse = ConvertMapsToPBRValues(material, 2.2f, TexCoords);

	float MaskHardness = saturate(dot(normalize(CamDir), Norm));
	float SphereMask = UE4SphereMask(normalize(CamDir), Norm, 5, MaskHardness);

	//vec3 CloserColour = vec3(0.023, 0.034, 0.036);
 	//vec3 FurtherColour = vec3(0.052, 0.07, 0.11);
	vec3 CloserColour = normalize(vec3(0.023, 0.034, 0.036));
 	vec3 FurtherColour = normalize(vec3(0.149, 0.3843, 0.8549) );
	parse.diffuse = mix(CloserColour, FurtherColour, pow(SphereMask, 0.03)) * 20;

	float RRatio = saturate(dot(vec3(0.0f, 0.0f, 1.0f), Norm));
	parse.roughness = mix(1.0f, 0.002f, RRatio);
	
	
	parse.ao = 0.1f;
	parse.metallic = 1.0f;

	vec3 L0 = ProgrammablePBR(Norm, View, r, L, parse, instense);
	//float roughRat = saturate(Bilerp(WorldPos.y, ActorPos.y, ActorPos.y + 50));
	vec3 ambient = vec3(0.03) * parse.diffuse * parse.ao;
    color = vec4(ambient + L0 , 0.0f);
	//color = vec4(roughRat);
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------