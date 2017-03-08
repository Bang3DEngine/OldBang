#include "SP.frag"
#include "DirectionalLight.glsl"

void Main()
{
    if (B_SampleReceivesLight())
    {
        vec4 diffColor = B_SampleDiffColor();
        vec3 dirLightApport = GetDirectionalLightColorApportation(
                                    B_InPositionWorld,
                                    B_InNormalWorld,
                                    diffColor.rgb,
                                    FRAG_IN_SHININESS(),
                                    B_LightForwardWorld,
                                    B_LightIntensity,
                                    B_LightColor.rgb,
                                    B_CameraPositionWorld.xyz);

        B_vout.color = vec4(B_SampleColor().rgb + dirLightApport, diffColor.a);
    }
}
