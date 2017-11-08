#define BANG_FRAGMENT
#define BANG_G
#include "Common.glsl"
#include "Ins.glsl"
#include "Outs.glsl"

bool B_UsePlainDiffuseColor;

void InitMain()
{
    B_UsePlainDiffuseColor  = false;
    B_FOut.Normal           = B_FIn_Normal;
    B_FOut.ReceivesLighting = B_MaterialReceivesLighting;
    B_FOut.Shininess        = B_MaterialShininess;
    B_FOut.Diffuse          = B_MaterialDiffuseColor;
    B_FOut.Color            = B_MaterialDiffuseColor;
}

void EndMain()
{
    B_GIn_Normal   = vec4(B_FOut.Normal.xyz * 0.5f + 0.5f, 0);
    B_GIn_Diffuse  = B_FOut.Diffuse;
    B_GIn_Misc     = vec4(B_FOut.ReceivesLighting ? 1.0 : 0.0,
                           B_FOut.Shininess,
                           0, 0);

    vec4 outColor;
    if (!B_UsePlainDiffuseColor)
    {
        float ambientLight = (B_MaterialReceivesLighting ? 0.1 : 1.0);
        outColor = vec4(B_FOut.Diffuse.rgb * B_FOut.Color.rgb * ambientLight,
                        B_FOut.Diffuse.a * B_FOut.Color.a);
    }
    else { outColor = B_FOut.Color; }

    B_FOut.Color  = vec4( mix(B_SampleColor().rgb, outColor.rgb, outColor.a), 1);
    B_GIn_Color = B_FOut.Color;
}

#include "Main.glsl"
