#define BANG_NO_STENCIL_TEST
#include "PR.frag"

bool IsSelected(vec2 offset)
{
    bool pixelSelected = texture2D(
                B_stencil_gout_fin,
                FRAG_IN_UV_SCREEN() + offset * B_pixel_step).w == 1.0f;
    return pixelSelected;
}

void Main()
{
    const vec4  outlineColor = vec4(1, 0.5, 0, 1);
    const float fillOpacity = 0.2f;
    const int   stroke = 1;

    bool isSelected = FRAG_IN_STENCIL() == 1;
    if (isSelected)
    {
        bool isOutline = false;
        for (int i = -stroke; i <= stroke && !isOutline; ++i)
        {
            for (int j = -stroke; j <= stroke && !isOutline; ++j)
            {
                isOutline = !IsSelected( vec2(i,j) );
            }
            if (isOutline) break;
        }

        if (isOutline)
        {
            B_vout.color = outlineColor;
        }
        else
        {
            B_vout.color += outlineColor * fillOpacity;
        }
    }
}
