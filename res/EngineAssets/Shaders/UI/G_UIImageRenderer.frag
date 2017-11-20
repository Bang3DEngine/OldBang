#include "UI.frag"

void main()
{
    vec4 texColor = vec4(1);
    if (B_HasTexture)
    {
        texColor = texture2D(B_Texture0, B_FIn_Uv);
        B_GIn_Color = B_MaterialDiffuseColor * texColor;
    }
    else
    {
        B_GIn_Color = B_MaterialDiffuseColor;
    }
}
