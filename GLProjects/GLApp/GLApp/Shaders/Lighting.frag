#version 330 core

struct Material
{
	sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light
{
vec3 direction;
vec3 position;
float cutOff;
float outerCutOff;

vec3 ambient;
vec3 diffuse;
vec3 specular;

float constant;
float linear;
float quadratic;

};


out vec4 color;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;


uniform vec3 viewPos;
uniform Material material;
uniform Light light;




void main ()
{
//Ambient Lighting 
vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

//Diffuse
vec3 norm = normalize(Normal);
//light pos  - frag pos, returns the direction the light is pointing
vec3 lightDir = normalize( light.position - FragPos);
//vec3 lightDir = normalize(-light.direction); //Directional light
float diff = max(dot(norm, lightDir), 0.0);
vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

//Specular
vec3 viewDir = normalize(viewPos - FragPos);
vec3 reflectDir = reflect(-lightDir, norm);
float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));


float theta = dot(lightDir, normalize(-light.direction));
float epsilon = (light.cutOff - light.outerCutOff);
float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0f);
diffuse *= intensity;
specular *= intensity;

//Attenuation (light fall off)
float  distance = length(light.position - FragPos);
float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

ambient *= attenuation;
diffuse *= attenuation;
specular *= attenuation;

color = vec4(ambient + diffuse + specular, 1.0f);



};
