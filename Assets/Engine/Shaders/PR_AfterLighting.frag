#include "PR.frag"

void main()
{
    InitMain();

    // ONLY FOR EDITOR

    // SELECTION OUTLINE EFFECT
    bool isSelected = texture2D(B_materialProps_gout_fin, B_vin.uv_screen).w > 0.5;
    if (isSelected)
    {
        B_vout.color = B_vin.color + vec4(0, 0.1, 0.3, 0);
    }
    //

    //

    EndMain();
}
