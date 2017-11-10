#define BANG_FRAGMENT
#define BANG_SP
#include "Common.glsl"
#include "Ins.glsl"
#include "Outs.glsl"

vec4 inColor;
void InitMain()
{
    inColor = B_SampleColor();
    B_FOut.Color = inColor; // Default value
}

void EndMain()
{
    B_FOut.Color = vec4( mix(inColor.rgb, B_FOut.Color.rgb, B_FOut.Color.a), 1.0);
    B_GIn_Color = B_FOut.Color;
}

void Main();
void main()
{
    InitCommon();
    InitMain();
    Main();
    EndMain();
}

