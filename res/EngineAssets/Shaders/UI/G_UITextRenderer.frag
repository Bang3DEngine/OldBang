#include "UI.frag"

void main()
{
    vec4 texColor = texture(B_Texture0, B_FIn_Uv);

    B_GIn_Normal = B_GIn_Misc = vec4(0);
    B_GIn_Color  = B_GIn_Diffuse = vec4(B_MaterialDiffuseColor.rgb,
                                        B_MaterialDiffuseColor.a * texColor.a);
}
