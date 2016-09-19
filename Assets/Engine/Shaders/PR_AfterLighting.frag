#include "PR.frag"

void main()
{
    InitMain();

    // ONLY FOR EDITOR

    // SELECTION OUTLINE EFFECT
    bool isSelected = texture2D(B_materialProps_gout_fin, B_vin.uv_screen).w > 0.5;
    if (isSelected)
    {
        B_vout.color = vec4(B_vin.color.rgb * 2.0, 1);
    }
    //

    //

    EndMain();
}
