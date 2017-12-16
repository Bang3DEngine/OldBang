#define BANG_FRAGMENT
#define BANG_G

#include "Common.glsl"

in vec3 B_FIn_Position;
in vec3 B_FIn_Normal;
in vec2 B_FIn_Uv;

layout(location = 0) out vec4 B_GIn_Color;

void main()
{
    B_GIn_Color = B_MaterialDiffuseColor;
}
