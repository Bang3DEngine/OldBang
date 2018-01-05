#include "PP.frag"
#include "DirectionalLight.glsl"

void main()
{
    vec4 diffColor = B_SampleDiffColor();
    if (B_SampleReceivesLight())
    {
        vec3 dirLightApport = GetDirectionalLightColorApportation(
                                    B_ComputeWorldPosition(),
                                    B_SampleNormal(),
                                    diffColor.rgb,
                                    B_SampleShininess(),
                                    B_LightForwardWorld,
                                    B_LightIntensity,
                                    B_LightColor.rgb,
                                    B_GetCameraPositionWorld() );

        B_GIn_Color = vec4(B_SampleColor().rgb + dirLightApport, diffColor.a);
    }
    else
    {
        // Should not arrive here, because pixels that do not receive light
        // are stenciled
        // TODO: This seems not to be being stenciled, fix this in behalf of performance
        // B_GIn_Color = vec4(1,0,0,1);
        B_GIn_Color = diffColor;
    }
}
