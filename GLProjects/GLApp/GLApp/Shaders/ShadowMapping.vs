#version 430 core

//-------------------------------------------------------------------

layout (location = 0) in vec3 position;

uniform mat4 lightSpaceMatrix;
uniform bool CSM;
uniform mat4 model;

//-------------------------------------------------------------------

void main()
{
    if(!CSM)
    {
        gl_Position = lightSpaceMatrix * model * vec4(position, 1.0); 
    }
    else
    {
        gl_Position = model * vec4(position, 1.0); 
    }
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
