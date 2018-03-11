#include "UI.frag"

void main()
{
    vec4 texColor = B_HasTexture ? texture(B_Texture0, B_FIn_Uv) : vec4(1);

    B_GIn_Normal = B_GIn_Misc = vec4(0);
    B_GIn_Color  = B_GIn_Diffuse = B_MaterialDiffuseColor * texColor;
}
