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
    B_GIn_NormalDepth  = vec4(B_Out_NormalWorld.xy, 0, 0);
    B_GIn_DiffColor    = B_Out_DiffColor;
    B_GIn_Misc         = vec4(B_Out_ReceivesLighting ? 1.0 : 0.0,
                              B_Out_Shininess,
                              B_IsSelected ? 1.0 : 0.0,
                              0);

    // Encode depth
    float depthHigh      = floor(B_Out_Depth * 1024);
    depthHigh           *= B_Out_NormalWorld.z >= 0.0 ? 1.0 : -1.0;
    float depthLow       = fract(B_Out_Depth * 1024);
    B_GIn_NormalDepth.z  = depthHigh;
    B_GIn_NormalDepth.w  = depthLow;

    float ambientLight = (B_MaterialReceivesLighting ? 0.1 : 1.0);
    vec3 outColor      = ambientLight * B_Out_DiffColor.rgb;
    B_Out_Color        = vec4( mix(B_SampleColor().rgb,
                                   outColor, B_Out_Color.a), 1);
    B_GIn_Color        = B_Out_Color;
}

#include "Main.glsl"
