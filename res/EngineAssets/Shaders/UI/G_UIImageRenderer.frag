#include "G.frag"

void Main()
{
    vec2 inUv = B_FIn_Uv;
    vec4 textureColor = vec4(1);
    vec4 tint = B_FOut.Diffuse;
    if (B_HasTexture)
    {
        textureColor = texture2D(B_Texture0, inUv);
        if (textureColor.a <= B_AlphaCutoff) { discard; }
    }
    B_FOut.Color = vec4(tint.rgb * textureColor.rgb, textureColor.a);
}
