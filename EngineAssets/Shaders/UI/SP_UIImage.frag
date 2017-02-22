#include "SP.frag"

uniform vec4 B_tint;

void Main()
{
    vec2 inUv = FRAG_IN_UV();
    vec4 textureColor = vec4(1);
    if (B_hasTexture)
    {
        textureColor = texture2D(B_texture_0, inUv);
    }
    B_vout.color = vec4(mix(textureColor, B_tint, B_tint.a).rgb, 1);
}
