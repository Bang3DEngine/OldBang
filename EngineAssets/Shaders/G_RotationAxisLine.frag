#include "G.frag"

uniform vec3  B_world_circleCenter;

void Main()
{
    vec3 fragPosWorld = FRAG_IN_POSITION_WORLD();
    vec3 circleLineNormal = normalize(fragPosWorld - B_world_circleCenter);
    vec3 camToCenter = normalize(B_world_circleCenter - B_position_camera);

    // Discard all those line points whose normal is facing away from camera
    if (dot(camToCenter, circleLineNormal) > -0.1) discard;

    B_vout.diffuseColor = vec4(B_material_diffuse_color.rgb, 1);
}
