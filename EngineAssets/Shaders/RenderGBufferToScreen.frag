#define BANG_SP
#define BANG_FRAGMENT
#include "Common.glsl"

void main()
{
    InitCommon();
    vec4 color   = B_SampleColor();
    gl_FragColor = vec4(color.rgb, 1);
}
