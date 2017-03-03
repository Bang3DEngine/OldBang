#include "SP.frag"
#include "PointLight.glsl"

void Main()
{
    if (FRAG_IN_RECEIVES_LIGHTING())
    {
        vec4 diffColor = FRAG_IN_DIFFUSE_COLOR();
        vec3 worldPos = B_ComputeWorldPosition( FRAG_IN_DEPTH() );
        vec3 pointLightApport = GetPointLightColorApportation(
                                      worldPos,
                                      FRAG_IN_NORMAL_WORLD(),
                                      diffColor.rgb,
                                      FRAG_IN_SHININESS(),
                                      B_light_position_world,
                                      B_light_intensity,
                                      B_light_range,
                                      B_light_color.rgb,
                                      B_position_camera.xyz);

        B_vout.color = vec4(FRAG_IN_COLOR().rgb + pointLightApport, diffColor.a);
    }
}
