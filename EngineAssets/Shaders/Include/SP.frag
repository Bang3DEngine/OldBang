#define BANG_FRAGMENT
#define BANG_SP
#include "Common.glsl"

vec4 inColor;
void InitMain()
{
    inColor = B_SampleColor();
    B_Out_Color = inColor; // Default value
}

void EndMain()
{
    B_Out_Color = vec4( mix(inColor.rgb, B_Out_Color.rgb, B_Out_Color.a), 1.0);
    B_GIn_Color = B_Out_Color;
}

// Custom main that checks stencil for this case
void Main();
void main()
{
    InitCommon();
    InitMain();
    Main();
    EndMain();
}

