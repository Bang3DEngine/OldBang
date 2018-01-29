#include "UI.frag"

void main()
{
    vec2 charAtlasUv = B_FIn_Uv;
    vec4 thisColor = texture2D(B_Texture0, charAtlasUv);

    B_GIn_Normal = B_GIn_Misc = vec4(0);
    B_GIn_Color  = B_GIn_Diffuse = vec4(B_MaterialDiffuseColor.rgb, thisColor.a);
}
