#version 430 core

//-------------------------------------------------------------------

layout (location = 0) in vec3 position;
 
uniform mat4 model; //converts local object coords to camera coords
uniform mat4 view; //converts normalised coordinates to window coordinates, aka what your window is
uniform mat4 projection; //converts those camera coordinates to normalised coordinates(0-1)
uniform vec3 inColour;

//-------------------------------------------------------------------

void main()
{

    gl_Position =  projection * view * model * vec4 (position, 1.0f);

};

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------