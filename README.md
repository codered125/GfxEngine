# OpenGL Forward / Deffered Rendering Engine

Decided to make this public, this is my current project and it is still ongoing.
The Graphics Engine currently has the following features implemented:
- [x] HDR
- [x] Directional, Point, and Spot lighting (Blinn/Phong)
- [x] Mesh importing (currently best supporting obj)
- [x] Texture Loading (best supporting mtls)
- [x] Frame buffers / Post Processing effects (Dynamic adjustments implemented)
- [x] GLSL Fragment / Vertex shader importing
- [x] Multisample anti-aliasing (4-16)
- [x] Physics Based Rendering (PBR)
- [x] Shadow Mapping (Not Cascading)
- [x] Render Target Revamp
- [x] Modularising my lighting and shading algorithms in prep for Deffered
- [x] Deffered Rendering - After PBR
- [x] Deffered Lighting - After Deffered Rendering
- [ ] Implementing Tesselation (PN Triangles)
- [ ] Port Gertsner water sim to engine
- [ ] Image Based Lighting PBR

The Graphics Engine currently utalises the current libraries:
- [x] ASSIMP
- [x] GLEW
- [x] GLFW
- [x] GLM
- [x] SOIL2
