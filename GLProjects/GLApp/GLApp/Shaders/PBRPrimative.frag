#version 330 core
#define M_PI 3.1415926535897932384626433832795
#define NUMBER_OF_POINT_LIGHTS 1

struct light
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
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


struct Material
{
    sampler2D  texture_diffuse;
    sampler2D  texture_specular;
	sampler2D  texture_normal;
	sampler2D  texture_height;
	sampler2D  texture_roughness;
	float shininess;
};

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;
in vec3 WorldPos;

out vec4 color;

uniform Material material;
uniform vec3 viewPos;
uniform DirLight dirLight;
uniform PointLight pointLights[NUMBER_OF_POINT_LIGHTS];

vec3 halfwayVec = vec3(0, 0, 0);

bool  blin = false;
 
vec3 calculateSpecularComponent();
vec3 BRDF(vec3 incomingLightDirection, vec3 viewDirection, vec3 surfaceNormal, float roughness);
vec3 getNextIncomingLightDir(int i);
float Normaldistributionfunction(vec3 inNormal, vec3 inHalwayVec, float inRoughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float k);
float Geometryfunction(float nDotV, float k);
vec3 FresnelEquation(float cosTheta, float F);
vec3 returnPointLightDirection(int i);
float saturate(float x) {return max(x, 0.0f);};
vec3 CalcDirLight( DirLight light, vec3 normal, vec3 viewDir );
vec3 getNormalFromMap();

void main()
{
	vec3 albedo     = pow(texture(material.texture_diffuse, TexCoords).rgb, vec3(2.2));
    //float metallic  = texture(metallicMap, TexCoords).r;
    float roughness = texture(material.texture_roughness, TexCoords).r; 
	vec3 norm = getNormalFromMap();
	vec3 viewDir = vec3(0, 0, 0);
	vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, material.shininess);


    viewDir = normalize( viewPos - FragPos );
	int steps = NUMBER_OF_POINT_LIGHTS; //100.0f;
	vec3 result;
	result += CalcDirLight( dirLight, norm, viewDir );
	float dW = 1.0 / steps;

	vec3 lightDir = returnPointLightDirection(0);

	float cosTheta = dot(lightDir , norm);

	for(int i = 0; i < steps; ++i)
	{
		vec3 incomingLightDirection = returnPointLightDirection(i);
		float distance = length(pointLights[i].position - WorldPos);
		float attenuation = 1.0 / (distance * distance);
		halfwayVec = normalize((incomingLightDirection  + viewDir) / length(incomingLightDirection + viewDir));
		
		//radiantIntensity = radiant flux (lightColour)/ solid angle (incominglightDirection)
		vec3 radiantIntensity = pointLights[i].diffuse * attenuation;
		

		result  += BRDF(incomingLightDirection, viewDir, norm, roughness) * radiantIntensity * dot(norm, incomingLightDirection) * dW;
	}
	color = vec4(result, 1.0f);
}


vec3 CalcDirLight( DirLight light, vec3 normal, vec3 viewDir )
{
    vec3 lightDir = normalize( -light.direction );
    
    // Diffuse shading
    float diff = max( dot( normal, lightDir ), 0.0 );
    
    // Specular shading (BlinPhong)
    vec3 reflectDir = reflect( -lightDir, normal );
	float spec = 0;
	if (blin)
	{
		vec3 halfWayDir = normalize(lightDir + viewDir);
		spec = pow(max(dot(normal, halfWayDir), 0.0f), material.shininess);
    }

	else
	{
		spec = pow( max( dot( viewDir, reflectDir ), 0.0 ), material.shininess );
	}
    
    // Combine results
    vec3 ambient = light.ambient * vec3( texture( material.texture_diffuse, TexCoords ) );
    vec3 diffuse = light.diffuse * diff * vec3( texture( material.texture_diffuse, TexCoords ) );
    vec3 specular = light.specular * spec * vec3( texture( material.texture_specular, TexCoords ) );
    return ( ambient + diffuse + specular );
}


vec3 calculateSpecularComponent()
{
	vec3 diffuse = pow(texture(material.texture_diffuse, TexCoords).rgb, vec3(2.2));
	vec3 F0 = vec3(0.04); 
    F0 = mix(F0, diffuse, material.shininess);
	vec3 viewDir = normalize( viewPos - FragPos );
	return FresnelEquation(max(dot(halfwayVec, viewDir), 0.0), 0.04);
}

vec3 BRDF(vec3 incomingLightDirection, vec3 viewDirection, vec3 surfaceNormal, float roughness)
{
	//kD * F lamber + kS * F cook-torrance

	//specular
	vec3 kS = calculateSpecularComponent();

	//diffuse
	vec3 kD = vec3(1.0f) - kS;
	
	vec3 diffuse = texture(material.texture_diffuse, TexCoords).rgb;
	vec3 fLambert = pow(diffuse, vec3(2.2)) / M_PI;
	vec3 L = incomingLightDirection;
	//vec3 F0 = vec3(0.04); 
    //F0 = mix(F0, albedo, metallic);
	vec3 viewDir = normalize( viewPos - FragPos );
	vec3 norm = getNormalFromMap();
	vec3 cNom = Normaldistributionfunction(norm, halfwayVec, texture(material.texture_roughness,TexCoords).r) * GeometrySmith (norm, viewDirection, L, texture(material.texture_roughness,TexCoords).r) *  saturate(FresnelEquation(max(dot(halfwayVec, viewDir), 0.0), 0.04 ));
	float denom = 4 * saturate((dot(viewDirection , surfaceNormal))) * saturate(dot(incomingLightDirection, surfaceNormal));

	float cookerTorrence = cNom / denom;
	vec3 output;
	return output = (kD * fLambert) + (kS * cookerTorrence);
}

vec3 getNextIncomingLightDir(int i)
{
	return  normalize( pointLights[i].position - FragPos ); 
}

//Trowbridge-Reitz GGX:
float Normaldistributionfunction(vec3 inNormal, vec3 inHalwayVec, float inRoughness)
{
	float a2 = inRoughness * inRoughness;
	float ndotH = saturate(dot(inNormal, inHalwayVec));
	float nominator =  a2;
	float denom =  ((ndotH * ndotH) * (a2 -1.0f) +1.0f);
	denom = M_PI * denom * denom;

	return nominator / denom;
}

//k = ((roughness +1) ^2) / 8;

float GeometrySmith(vec3 N, vec3 V, vec3 L, float k)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx1 = Geometryfunction(NdotV, k);
    float ggx2 = Geometryfunction(NdotL, k);
	
    return ggx1 * ggx2;
}

//Schlick-GGX:
float Geometryfunction(float nDotV, float k)
{
	float r = (texture(material.texture_roughness,TexCoords).r + 1.0);
    float outk = (r*r) / 8.0;

	float denom = nDotV * (1 - outk) + outk;
	return nDotV / denom;
}


//With cosTheta being the dot product result between the surface's normal n and the view direction v.
vec3 FresnelEquation(float cosTheta, float F)
{
	return vec3(saturate(F + (1.0 - F) * pow(1.0 - cosTheta, 5) );
}

vec3 returnPointLightDirection(int i)
{
	   return normalize( pointLights[i].position - FragPos ); 
}

vec3 getNormalFromMap()
{
	//normalise it to 0-1
    vec3 tangentNormal = texture(material.texture_normal, TexCoords).xyz * 2.0 - 1.0;

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