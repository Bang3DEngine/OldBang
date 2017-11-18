#include "G.frag"

void main()
{
    vec4 texColor = vec4(1);
    if (B_HasTexture)
    {
        texColor = texture2D(B_Texture0, B_FIn_Uv);
        if (texColor.a <= B_AlphaCutoff) { discard; }
    }

    B_GIn_Misc = vec4(0,0,0,0);
    B_GIn_Normal = vec4(0,0,1,0);
    B_GIn_Diffuse = vec4(B_MaterialDiffuseColor * texColor);
    B_GIn_Color = MixedWithBackground(B_GIn_Diffuse);
}
