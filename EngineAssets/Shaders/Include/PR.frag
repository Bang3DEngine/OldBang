#define BANG_FRAGMENT
#define BANG_PR
#include "Uniforms.glsl"

struct B_VertexOut
{
    vec4 color;
};

B_VertexOut B_vout;

out vec4 B_color_gout_gin; // Accumulated color


void InitMain()
{
    // Default value
    B_vout.color = FRAG_IN_COLOR();
}

void EndMain()
{
    vec4 inColor = FRAG_IN_COLOR();
    B_vout.color = vec4( mix(inColor.rgb, B_vout.color.rgb, B_vout.color.a), B_vout.color.a);
    B_color_gout_gin = B_vout.color;
}

// Custom main that checks stencil for this case
void Main();
void main()
{
    InitMain();

    #ifdef BANG_NO_STENCIL_TEST
    bool passStencil = true;
    #else
    bool passStencil = (!B_stencilTestEnabled || FRAG_IN_STENCIL() == 1);
    #endif
    if (passStencil)
    {
        Main();
    }

    EndMain();
}

