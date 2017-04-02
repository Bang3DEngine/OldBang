#define BANG_FRAGMENT
#define BANG_SP
#include "Common.glsl"

void InitMain()
{
    B_Out_Color = B_SampleColor(); // Default value
}

void EndMain()
{
    vec4 inColor = B_SampleColor();
    B_Out_Color = vec4( mix(inColor.rgb, B_Out_Color.rgb, B_Out_Color.a),
                       B_Out_Color.a);
    B_GIn_Color = B_Out_Color;
}

// Custom main that checks stencil for this case
void Main();
void main()
{
    InitCommon();

    #ifdef BANG_NO_STENCIL_TEST
    bool passStencil = true;
    #else
    bool passStencil = (!B_StencilTestEnabled || B_SampleStencil() == 1);
    #endif
    if (passStencil)
    {
        InitMain();
        Main();
        EndMain();
    }
    else { discard; }
}

