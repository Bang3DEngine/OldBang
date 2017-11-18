#include "G.frag"

void main()
{
    vec4 texColor = vec4(1);
    if (B_HasTexture)
    {
        texColor = texture2D(B_Texture0, B_FIn_Uv);
        if (texColor.a <= B_AlphaCutoff) discard;
    }

    B_GIn_Diffuse  = B_MaterialDiffuseColor * texColor;
    B_GIn_Normal   = vec4(B_FIn_Normal.xyz * 0.5f + 0.5f, 0);
    B_GIn_Misc     = vec4(B_MaterialReceivesLighting ? 1.0 : 0.0,
                          B_MaterialShininess, 0, 0);

    float ambientLight = (B_MaterialReceivesLighting ? 0.1 : 1.0);
    vec4 outColor = vec4(B_MaterialDiffuseColor.rgb * texColor.rgb * ambientLight,
                         B_MaterialDiffuseColor.a * texColor.a);

    B_GIn_Color = MixedWithBackground(outColor);
}
