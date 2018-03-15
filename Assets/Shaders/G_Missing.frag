#include "G.frag"

void main()
{
    B_GIn_Diffuse  = B_MaterialDiffuseColor * texColor;
    B_GIn_Normal   = vec4(0, 0, 0, 1);
    B_GIn_Misc.x   = 0;
    B_GIn_Color = vec4(1, 0, 1, 1);
}
