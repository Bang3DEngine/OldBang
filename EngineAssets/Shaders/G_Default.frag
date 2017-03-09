#include "G.frag"

void Main()
{
    vec4 texColor = vec4(1);
    if (B_HasTexture)
    {
        texColor = texture2D(B_Texture0, B_FragIn_Uv);
        if (texColor.a <= B_AlphaCutoff) discard;
    }
    B_Out_DiffColor = B_MaterialDiffuseColor * texColor;
}
