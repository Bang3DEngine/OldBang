#include "G.frag"

uniform vec3  B_world_circleCenter;

void Main()
{
    vec3 fragPosWorld = B_InPositionWorld;
    vec3 circleLineNormal = normalize(fragPosWorld - B_world_circleCenter);
    vec3 camToCenter = normalize(B_world_circleCenter - B_CameraPositionWorld);

    // Discard all those line points whose normal is facing away from camera
    if (dot(camToCenter, circleLineNormal) > -0.1) discard;

    B_vout.diffuseColor = vec4(B_MaterialDiffuseColor.rgb, 1);
}
