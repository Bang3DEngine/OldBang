#define BANG_SP
#define BANG_VERTEX
#include "Common.glsl"

layout(location = 0) in vec3 B_VIn_Position;
layout(location = 1) in vec3 B_VIn_Normal;
layout(location = 2) in vec2 B_VIn_Uv;

out vec3 B_FIn_Position;
out vec3 B_FIn_Normal;
out vec2 B_FIn_Uv;
