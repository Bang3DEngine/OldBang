#include "D2G.frag"

void Main()
{
    if (B_hasTexture)
    {
        // ALPHA CUTTOFF
        vec4 texColor = texture2D(B_texture_0, B_vin.uv);
        if (texColor.a <= B_alphaCuttoff) B_vout.diffuseColor = vec4(0,1,0,1);
        //

        vec4 diffColor = B_material_diffuse_color;

        // MIX DIFFUSE_COLOR AND TEXTURE_COLOR
        B_vout.diffuseColor = texColor * diffColor;
        //
    }
}
