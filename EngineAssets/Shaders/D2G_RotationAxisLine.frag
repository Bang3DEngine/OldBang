#include "D2G.frag"

uniform vec3  B_world_circleCenter;
uniform float B_boundingSphereRadius;

void Main()
{
    vec3 world_cameraUp     = normalize( (B_matrix_view * vec4(0,1,0,0)).xyz );
    vec3 world_maxDistPoint = B_world_circleCenter + world_cameraUp * B_boundingSphereRadius;

    float maxDist = distance(B_position_camera, world_maxDistPoint);
    float dToV    = distance(B_position_camera, B_position_world_vout_fin.xyz);

    if (dToV > maxDist) discard;

    B_vout.diffuseColor = vec4(B_material_diffuse_color.rgb, 1);
}
