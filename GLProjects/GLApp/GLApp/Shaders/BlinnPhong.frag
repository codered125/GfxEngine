#version 330 core
#define NUMBER_OF_POINT_LIGHTS 1

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


in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
out vec4 color;

uniform vec3 viewPos;
uniform DirLight dirLight;
uniform PointLight pointLights[NUMBER_OF_POINT_LIGHTS];
uniform SpotLight spotLight;
uniform Material material;
uniform samplerCube skybox;
uniform float Time;
bool  blin = false;
vec3 CalcDirLight( DirLight light, vec3 normal, vec3 viewDir );
vec3 CalcPointLight( PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir );
vec3 CalcSpotLight( SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir );

void main ()
{
	//color = vec4(texture(material.texture_diffuse,  TexCoords));
    // Properties
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize( viewPos - FragPos );
	 
    vec3 result;
	result += CalcDirLight( dirLight, norm, viewDir );    
    for ( int i = 0; i < NUMBER_OF_POINT_LIGHTS; i++ )
    {
        result += CalcPointLight( pointLights[i], norm, FragPos, viewDir );
    }
    result += CalcSpotLight( spotLight, norm, FragPos, viewDir );
	color = vec4( result, 1.0 );

};




// Calculates the color when using a directional light.
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





// Calculates the color when using a point light.
vec3 CalcPointLight( PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir )
{
	
    vec3 lightDir = normalize( light.position - fragPos );
    
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
    // Attenuation
    float distance = length( light.position - fragPos );
    float attenuation = 1.0f / ( light.constant + light.linear * distance + light.quadratic * ( distance * distance ) );
	attenuation *= 3;
    
    // Combine results
    vec3 ambient = light.ambient * vec3( texture( material.texture_diffuse, TexCoords ) );
    vec3 diffuse = light.diffuse * diff * vec3( texture( material.texture_diffuse, TexCoords ) );
    vec3 specular = light.specular * spec * vec3( texture( material.texture_specular, TexCoords ) );
    

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;


	vec3 returnValue = ambient + diffuse + specular;
	vec3 zero = vec3(0.0f, 0.0f, 0.0f);
	//float TimeRemaining = frac(Time);
	returnValue = zero + ((returnValue - zero) * Time);
    
    return (returnValue);
}


// Calculates the color when using a spot light.
vec3 CalcSpotLight( SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir )
{
    vec3 lightDir = normalize( light.position - fragPos );
    
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
    
    // Attenuation
    float distance = length( light.position - fragPos );
    float attenuation = 1.0f / ( light.constant + light.linear * distance + light.quadratic * ( distance * distance ) );
    
    // Spotlight intensity
    float theta = dot( lightDir, normalize( -light.direction ) );
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp( ( theta - light.outerCutOff ) / epsilon, 0.0, 1.0 );
    
    // Combine results
    vec3 ambient = light.ambient * vec3( texture( material.texture_diffuse, TexCoords ) );
    vec3 diffuse = light.diffuse * diff * vec3( texture( material.texture_diffuse, TexCoords ) );
    vec3 specular = light.specular * spec * vec3( texture( material.texture_specular, TexCoords ) );
    
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    
    return ( ambient + diffuse + specular );
}