#include Shaders/TestIncludeSupport1.glsl

vec3 Main()
{
  vec3 Colour = TestIncludeSupport1 + TestIncludeSupport2 + vec3(0.0f, 0.0f, 1.0f );
}