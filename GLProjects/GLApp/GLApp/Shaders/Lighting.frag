#version 330 core

out vec4 color;
in vec3 FragPos;
in vec3 Normal;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 objectColour;
uniform vec3 lightColour;



void main ()
{
//Ambient Lighting 
float ambientStrenght = 0.1f;
vec3 ambient = ambientStrenght * lightColour;

//Diffuse
vec3 norm = normalize(Normal);
//light pos  - frag pos, returns the direction the light is pointing
vec3 lightDir = normalize(lightPos - FragPos);
float diff = max(dot(norm, lightDir), 0.0);
vec3 diffuse = diff * lightColour;

//Specular
float specularStr = 2.0f;
vec3 viewDir = normalize(viewPos - FragPos);
vec3 reflectDir = reflect(-lightDir, norm);
float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
vec3 specular = specularStr * spec * lightColour;

vec3 result = (ambient + diffuse + specular) * objectColour;
color = vec4(result, 1.0f);



};
