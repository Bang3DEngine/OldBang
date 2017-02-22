#include "G.frag"

void Main()
{
    vec4 diffColor = B_material_diffuse_color;
    if (B_hasTexture)
    {
        // ALPHA CUTOFF
        vec2 fragmentUv = FRAG_IN_UV();
        vec4 texColor = texture2D(B_texture_0, fragmentUv);
        if (texColor.a <= B_alphaCutoff) discard;
        //

        // MIX DIFFUSE_COLOR AND TEXTURE_COLOR
        B_vout.diffuseColor = texColor * diffColor;
        //
    }
    else
    {
        B_vout.diffuseColor = diffColor;
    }
}
