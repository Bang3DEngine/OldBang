#include "G.frag"

void Main()
{
    vec2 charAtlasUv = B_FragIn_Uv;
    vec4 charColor   = texture2D(B_Texture0, charAtlasUv);

    if (charColor.a == 0.0) { discard; }
    B_Out_Color = charColor;
}
