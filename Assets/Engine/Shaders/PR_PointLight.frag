#include "PR.frag"
#include "PointLight.glsl"

void main()
{
    InitMain();

    if (B_vin.receivesLighting)
    {
        vec3 pointLightApport = GetPointLightColorApportation(
                                      B_vin.position_world,
                                      B_vin.normal_world,
                                      B_vin.diffuseColor.rgb,
                                      B_vin.shininess,
                                      B_light_position_world,
                                      B_light_intensity,
                                      B_light_range,
                                      B_light_color.rgb,
                                      B_position_camera.xyz);

        B_vout.color = vec4(B_vin.color.rgb + pointLightApport,
                            B_vin.diffuseColor.a);
    }

    EndMain();
}
