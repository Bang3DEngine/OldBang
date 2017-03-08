#include "SP.frag"
#include "PointLight.glsl"

void Main()
{
    if (B_SampleReceivesLight())
    {
        vec4 diffColor = B_SampleDiffColor();
        vec3 worldPos = B_ComputeWorldPosition( B_SampleDepth() );
        vec3 pointLightApport = GetPointLightColorApportation(
                                      worldPos,
                                      B_SampleNormal(),
                                      diffColor.rgb,
                                      B_SampleShininess(),
                                      B_LightPositionWorld,
                                      B_LightIntensity,
                                      B_light_range,
                                      B_LightColor.rgb,
                                      B_CameraPositionWorld.xyz);

        B_vout.color = vec4(B_SampleColor().rgb + pointLightApport, diffColor.a);
    }
}
