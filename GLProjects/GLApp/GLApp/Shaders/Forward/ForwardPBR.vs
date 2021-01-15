 #version 430 core
layout (location = 0) in vec3 position; 
layout (location = 1) in vec3 normal; 
layout (location = 2) in vec2 texCoords; 
layout (location = 3) in vec3 Tangent; 
layout (location = 4) in vec3 Bitangent; 

out V2F
{
    vec3 Normal;
    vec2 TexCoords;
    vec3 WorldPos;
    vec4 FragPosLightSpace;
} 
vs_Out;

uniform mat4 model; //converts local object coords to camera coords
uniform mat4 view; //converts normalised coordinates to window coordinates, aka what your window is
uniform mat4 projection; //converts those camera coordinates to normalised coordinates(0-1)
uniform mat4 lightSpaceMatrix;

void main()
{
    gl_Position =  projection * view * model * vec4 (position, 1.0f);
    vs_Out.WorldPos = vec3(model * vec4(position, 1.0f));
    vs_Out.Normal = mat3(transpose(inverse(model))) * normal;
    vs_Out.TexCoords = texCoords;
    vs_Out.FragPosLightSpace = lightSpaceMatrix * model * vec4(position, 1.0f); 
};