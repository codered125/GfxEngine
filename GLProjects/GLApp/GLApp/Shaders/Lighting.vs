#version 330 core
layout (location = 0) in vec3 position; 
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;

uniform mat4 model; //converts local object coords to camera coords
uniform mat4 view; //converts normalised coordinates to window coordinates, aka what your window is
uniform mat4 projection; //converts those camera coordinates to normalised coordinates(0-1)

void main()
{

gl_Position =  projection * view * model * vec4 (position, 1.0f);
FragPos = vec3(model * vec4(position, 1.0f));
Normal = mat3(transpose(inverse(model))) * normal;
TexCoords = texCoords;
};