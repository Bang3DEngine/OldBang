#include "PP.frag"
#include "DirectionalLight.glsl"

void main()
{
    vec4 originalColor = B_SampleColor();
    vec3 pixelPosWorld = B_ComputeWorldPosition();
    if (B_SampleReceivesLight())
    {
        float shadowness = GetFragmentShadowness(pixelPosWorld);
        // B_GIn_Color = vec4( vec3(shadowness), 1 ); return; // Debug
        if (shadowness < 0.5f)
        {
            vec4 diffColor = B_SampleDiffColor();
            vec3 dirLightApport = GetDirectionalLightColorApportation(
                                        pixelPosWorld,
                                        B_SampleNormal(),
                                        diffColor.rgb,
                                        B_SampleShininess(),
                                        B_LightForwardWorld,
                                        B_LightIntensity,
                                        B_LightColor.rgb,
                                        B_GetCameraPositionWorld() );

            B_GIn_Color = vec4(originalColor.rgb + dirLightApport, diffColor.a);
        }
        else
        {
            B_GIn_Color = originalColor;
        }
    }
    else
    {
        // Should not arrive here, because pixels that do not receive light
        // are stenciled
        // TODO: This seems not to be being stenciled, fix this
        // B_GIn_Color = vec4(1,0,0,1);
        B_GIn_Color = originalColor;
    }
}
