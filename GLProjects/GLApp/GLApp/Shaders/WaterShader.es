#version 430 core

//-------------------------------------------------------------------

layout(triangles, equal_spacing, ccw) in;

uniform mat4 gVP;
uniform sampler2D gDisplacementMap;
uniform float gDispFactor;

in vec3 WorldPos_ES_in[];
in vec2 TexCoord_ES_in[];
in vec3 Normal_ES_in[];

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

//-------------------------------------------------------------------

vec2 interpolate2D(vec2 v0, vec2 v1, vec2 v2)
{
    return vec2(gl_TessCoord.x) * v0 + vec2(gl_TessCoord.y) * v1 + vec2(gl_TessCoord.z) * v2;
}

//-------------------------------------------------------------------

vec3 interpolate3D(vec3 v0, vec3 v1, vec3 v2)
{
    return vec3(gl_TessCoord.x) * v0 + vec3(gl_TessCoord.y) * v1 + vec3(gl_TessCoord.z) * v2;
}

//-------------------------------------------------------------------

void main()
{
    // Interpolate the attributes of the output vertex using the barycentric coordinates
    TexCoords = interpolate2D(TexCoord_ES_in[0], TexCoord_ES_in[1], TexCoord_ES_in[2]);
    Normal = interpolate3D(Normal_ES_in[0], Normal_ES_in[1], Normal_ES_in[2]);
    Normal = normalize(Normal);
    FragPos = interpolate3D(WorldPos_ES_in[0], WorldPos_ES_in[1], WorldPos_ES_in[2]);


    // Displace the vertex along the normal
    float Displacement = texture(gDisplacementMap, TexCoords.xy).x;
    FragPos += Normal * Displacement * gDispFactor;
    gl_Position = gVP * vec4(FragPos, 1.0);
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------