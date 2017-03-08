#define BANG_FRAGMENT
#define BANG_SP
#include "Uniforms.glsl"

struct B_VertexOut
{
    vec4 color;
};

B_VertexOut B_vout;

out vec4 B_color_gout_gin; // Accumulated color

void InitMain()
{
    B_vout.color = B_SampleColor(); // Default value
}

void EndMain()
{
    vec4 inColor = B_SampleColor();
    B_vout.color = vec4( mix(inColor.rgb, B_vout.color.rgb, B_vout.color.a),
                         B_vout.color.a);
    B_color_gout_gin = B_vout.color;
}

// Custom main that checks stencil for this case
void Main();
void main()
{
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

