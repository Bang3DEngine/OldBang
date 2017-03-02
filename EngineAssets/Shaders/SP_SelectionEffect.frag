#define BANG_NO_STENCIL_TEST
#include "SP.frag"

void Main()
{
    bool isOutsideStencil = (FRAG_IN_STENCIL() <= 0.5);
    if (isOutsideStencil)
    {
        const vec4  outlineColor = vec4(1, 0.5, 0, 1);
        const float fillOpacity  = 0.0f;
        const int   stroke       = 2;
        vec2  screenUv           = FRAG_IN_UV_SCREEN();
        vec2  ps                 = B_buffer_pixel_step;

        float closestDepth = 0.0f;
        bool isOutline = false;
        for (int i = -stroke; i <= stroke && !isOutline; ++i)
        {
            for (int j = -stroke; j <= stroke && !isOutline; ++j)
            {
                vec2 uv = screenUv + vec2(i,j)*ps;
                isOutline = texture2D(B_misc_gout_fin, uv).a > 0.5f;
                if (isOutline)
                {
                    closestDepth = texture2D(B_misc_gout_fin, uv).b;
                }
            }
        }

        if (isOutline)
        {
            float pixelDepth   = FRAG_IN_DEPTH();
            float depthFade  = (pixelDepth < closestDepth) ? 0.4f : 1.0f;
            B_vout.color = vec4(outlineColor.rgb, outlineColor.a * depthFade);
        }
    }
}
