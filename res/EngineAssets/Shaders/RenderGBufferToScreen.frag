#define BANG_SP
#define BANG_FRAGMENT
#include "Common.glsl"
#include "Ins.glsl"
#include "Outs.glsl"

void main()
{
    InitCommon();
    vec4 color = B_SampleColor();
    gl_FragColor = color;
    gl_FragDepth = B_SampleDepth();
}
