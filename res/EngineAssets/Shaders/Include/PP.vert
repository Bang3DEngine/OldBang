#define BANG_SP
#define BANG_VERTEX
#include "Common.glsl"

layout(location = B_LOC_VIN_POSITION) in vec3 B_VIn_Position;
layout(location = B_LOC_VIN_NORMAL)   in vec3 B_VIn_Normal;
layout(location = B_LOC_VIN_UV)       in vec2 B_VIn_Uv;

out vec3 B_FIn_Position;
out vec3 B_FIn_Normal;
out vec2 B_FIn_Uv;
