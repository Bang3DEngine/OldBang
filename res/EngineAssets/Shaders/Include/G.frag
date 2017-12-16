#define BANG_FRAGMENT
#define BANG_G

#include "Common.glsl"

in vec3 B_FIn_Position;
in vec3 B_FIn_Normal;
in vec2 B_FIn_Uv;

layout(location = 0) out vec4 B_GIn_Normal;
layout(location = 1) out vec4 B_GIn_Diffuse;
layout(location = 2) out vec4 B_GIn_Misc;
layout(location = 3) out vec4 B_GIn_Color;
