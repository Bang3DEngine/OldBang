#include "G.frag"

void Main()
{
    vec4 texColor = vec4(1);
    if (B_hasTexture)
    {
        vec2 fragmentUv = FRAG_IN_UV();
        texColor = texture2D(B_texture_0, fragmentUv);
        if (texColor.a <= B_alphaCutoff) discard;
    }
    B_vout.diffuseColor = B_material_diffuse_color * texColor;
}
