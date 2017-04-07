#define BANG_FRAGMENT
#define BANG_G
#include "Common.glsl"

void InitMain()
{
    B_Out_NormalWorld      = B_FragIn_NormalWorld;
    B_Out_ReceivesLighting = B_MaterialReceivesLighting;
    B_Out_Shininess        = B_MaterialShininess;
    B_Out_DiffColor        = B_MaterialDiffuseColor;
    B_Out_Depth            = gl_FragCoord.z;
    B_Out_Color            = B_MaterialDiffuseColor;
}

void EndMain()
{
    B_GIn_NormalWorld  = vec4(B_Out_NormalWorld, 0);
    B_GIn_DiffColor    = B_Out_DiffColor;
    B_GIn_Misc         = vec4(B_Out_ReceivesLighting ? 1 : 0,
                              B_Out_Shininess,
                              B_Out_Depth,
                              0);

    float ambientLight = (B_MaterialReceivesLighting ? 0.1 : 1.0);
    vec3 outColor      = ambientLight * B_Out_DiffColor.rgb;
    B_Out_Color        = vec4( mix(B_SampleColor().rgb,
                                   outColor, B_Out_Color.a), 1);
    B_GIn_Color        = B_Out_Color;
}

#include "Main.glsl"
