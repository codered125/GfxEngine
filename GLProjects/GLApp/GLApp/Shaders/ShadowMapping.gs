#version 430 core

//-------------------------------------------------------------------

//Shader runs in parrallel and is instanced, current instance can be reffered to with gl_InvocationID
layout(triangles, invocations = 5) in;
layout(triangle_strip, max_vertices = 3) out;
    
layout (std140, binding = 0) uniform LightSpaceMatrices
{
    mat4 LightSpaceMatricesArray[10];
};

//-------------------------------------------------------------------

void main()
{
    for(int i = 0; i <3; ++i)
    {
        gl_Position = LightSpaceMatricesArray[gl_InvocationID] * gl_in[i].gl_Position;
        gl_Layer = gl_InvocationID;
        EmitVertex();
    }
    EndPrimitive();
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------