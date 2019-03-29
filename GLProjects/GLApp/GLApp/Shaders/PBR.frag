#version 330 core
#define M_PI 3.1415926535897932384626433832795

struct Material
{
    sampler2D  texture_diffuse;
    sampler2D  texture_specular;
	sampler2D  texture_normal;
	sampler2D  texture_height;
    float shininess;
};

struct DirLight
{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight
{
    float constant;
    float linear;
    float quadratic;

	vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight
{
    float cutOff;
    float outerCutOff;
    float constant;
    float linear;
    float quadratic;
    
	vec3 position;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};


in vec3 TexCoords;
in vec3 FragPos;
in vec3 Normal;
in vec3 WorldPos;

out vec4 color;

uniform Material material;
uniform samplerCube skybox;
uniform vec3 viewPos;
uniform DirLight dirLight;
uniform PointLight pointLights[NUMBER_OF_POINT_LIGHTS];


void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize( viewPos - FragPos );
	


	int steps = 100;

	float dW = 1.0 / steps;
	vec3 halfwayVec = (incomingLightDirection  + viewDirection) / length(incomingLightDirection + viewDirection);

	//radiantIntensity = radiant flux (lightColour)/ solid angle (incominglightDirection)
	float radiantIntensity = lightColour / incomingLightDirection;

	float cosTheta = dot(lightDir , surfaceNormal);
	//float result
	//result  += BRDF(FragPos, incomingLightDirection, viewDirection) * L(fragPos, incomingLightDirection) * dot(surfaceNormal, incomingLightDirection) * dW
}


float calculateSpecularComponent()
{
	return 0.0f;
}

float BRDF( vec3 incomingLightDirection, vec3 viewDirection, vec3 surfaceNormal, float roughness)
{
		//specular
	float kS = calculateSpecularComponent();
	
	//diffuse
	float kD = 1.0f - kS;

	float output = (kD * (texture(material.texture_diffuse, TexCoords) / M_PI)) + kS * 
}

vec3 getNextIncomingLightDir(int i)
{
	return pointLights[i];
}