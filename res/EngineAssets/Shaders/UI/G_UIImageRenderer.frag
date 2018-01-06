#include "UI.frag"

void main()
{
    vec4 texColor = B_HasTexture ? texture2D(B_Texture0, B_FIn_Uv) : vec4(1);
    B_GIn_Color = B_MaterialDiffuseColor * texColor;
}
