#include "UI.frag"

void main()
{
    vec4 texColor = vec4(1);
    if (B_HasTexture)
    {
        texColor = texture2D(B_Texture0, B_FIn_Uv);
        if (texColor.a <= B_AlphaCutoff) { discard; }
    }

    B_GIn_Color = MixedWithBackground(B_MaterialDiffuseColor * texColor);
}
