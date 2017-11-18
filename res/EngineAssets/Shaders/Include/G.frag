#define BANG_FRAGMENT
#define BANG_G
#include "Common.glsl"

in vec3 B_FIn_Position;
in vec3 B_FIn_Normal;
in vec2 B_FIn_Uv;

layout(location = B_LOC_GBUFFER_OUT_NORMAL)  out vec4 B_GIn_Normal;
layout(location = B_LOC_GBUFFER_OUT_DIFFUSE) out vec4 B_GIn_Diffuse;
layout(location = B_LOC_GBUFFER_OUT_MISC)    out vec4 B_GIn_Misc;
layout(location = B_LOC_GBUFFER_OUT_COLOR)   out vec4 B_GIn_Color;
