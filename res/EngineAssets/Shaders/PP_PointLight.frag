#include "PP.frag"
#include "PointLight.glsl"

void main()
{
    vec4 diffColor = B_SampleDiffColor();
    if (B_SampleReceivesLight())
    {
        vec3 worldPos = B_ComputeWorldPosition( B_SampleDepth() );
        vec3 pointLightApport = GetPointLightColorApportation(
                                      worldPos,
                                      B_SampleNormal(),
                                      diffColor.rgb,
                                      B_SampleShininess(),
                                      B_LightPositionWorld,
                                      B_LightIntensity,
                                      B_LightRange,
                                      B_LightColor.rgb,
                                      B_GetCameraPositionWorld() );

        B_GIn_Color = vec4(B_SampleColor().rgb + pointLightApport, diffColor.a);
    }
    else
    {
        // Should not arrive here, because pixels that do not receive light
        // are stenciled
        B_GIn_Color = vec4(1,0,0,1);
    }
}
