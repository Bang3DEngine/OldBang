#include "UI.frag"

void main()
{
    if (B_HasTexture)
    {
        B_GIn_Color = B_MaterialDiffuseColor * texture2D(B_Texture0, B_FIn_Uv);
    }
    else
    {
        B_GIn_Color = B_MaterialDiffuseColor;
    }
}
