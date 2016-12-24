#include "D2G.frag"

void Main()
{
    if (B_hasTexture)
    {
        vec4 texColor = texture2D(B_texture_0, B_vin.uv);

        // MIX DIFFUSE_COLOR AND TEXTURE_COLOR ( And text alpha )
        B_vout.diffuseColor = texColor * B_material_diffuse_color;
        //
    }
}
