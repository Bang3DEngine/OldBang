#include "G.frag"

void Main()
{
    vec4 texColor = vec4(1);
    if (B_HasTexture)
    {
        texColor = texture2D(B_Texture0, B_uv_world_vout_fin);
        if (texColor.a <= B_AlphaCutoff) discard;
    }
    B_vout.diffuseColor = B_MaterialDiffuseColor * texColor;
}
