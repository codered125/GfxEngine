#version 430 core
layout (location = 0) in vec3 Position;

out vec3 LocalPosition;

uniform mat4 view; //converts normalised coordinates to window coordinates, aka what your window is
uniform mat4 projection; //converts those camera coordinates to normalised coordinates(0-1)

void main()
{

gl_Position =  projection * view * vec4 (Position, 1.0f);
LocalPosition = Position;

};