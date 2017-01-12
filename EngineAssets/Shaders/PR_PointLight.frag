#include "PR.frag"
#include "PointLight.glsl"

void Main()
{
    if (FRAG_IN_RECEIVES_LIGHTING())
    {
        vec3 pointLightApport = GetPointLightColorApportation(
                                      FRAG_IN_POSITION_WORLD(),
                                      FRAG_IN_NORMAL_WORLD(),
                                      FRAG_IN_DIFFUSE_COLOR().rgb,
                                      FRAG_IN_SHININESS(),
                                      B_light_position_world,
                                      B_light_intensity,
                                      B_light_range,
                                      B_light_color.rgb,
                                      B_position_camera.xyz);

        B_vout.color = vec4(FRAG_IN_COLOR().rgb + pointLightApport, FRAG_IN_DIFFUSE_COLOR().a);
    }
}
