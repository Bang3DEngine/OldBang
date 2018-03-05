#define BANG_SP
#define BANG_FRAGMENT
#include "Common.glsl"

layout(location = 0) out vec4 B_GIn_Color;

void main()
{
    vec2 uv = GetViewportUv() * B_UvMultiply + B_UvOffset;

    vec4 color = B_SampleColor(uv);
    gl_FragColor = color;
    gl_FragDepth = B_SampleDepth(uv);
}
