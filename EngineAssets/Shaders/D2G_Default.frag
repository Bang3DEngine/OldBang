#include "D2G.frag"

void Main()
{
    vec4 diffColor = B_material_diffuse_color;
    if (B_hasTexture)
    {
        // ALPHA CUTTOFF
        vec4 texColor = texture2D(B_texture_0, B_vin.uv);
        if (texColor.a <= B_alphaCuttoff) discard;
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
