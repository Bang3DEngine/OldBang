#define BANG_SP
#define BANG_FRAGMENT
#include "Common.glsl"

struct B_FragmentOut
{
    vec4 Color;
}
B_FOut;

layout(location = 0) out vec4 B_GIn_Color;

void main()
{
    vec4 color = B_SampleColor();
    gl_FragColor = color;
    gl_FragDepth = B_SampleDepth();
}
