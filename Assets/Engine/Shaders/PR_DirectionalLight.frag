#include "PR.frag"
#include "DirectionalLight.glsl"

void main()
{
    InitMain();

    if (!IsEmpty() && B_vin.receivesLighting)
    {
        vec3 dirLightApport = GetDirectionalLightColorApportation(
                                    B_vin.position_world,
                                    B_vin.normal_world,
                                    B_vin.diffuseColor.rgb,
                                    B_vin.shininess,
                                    B_light_forward_world,
                                    B_light_intensity,
                                    B_light_color.rgb,
                                    B_position_camera.xyz);

        B_vout.color = vec4(B_vin.color.rgb + dirLightApport,
                            B_vin.diffuseColor.a);
    }

    EndMain();
}
