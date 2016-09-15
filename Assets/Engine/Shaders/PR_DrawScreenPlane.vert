#include "PR.vert"

in vec3 B_position_raw_vin; // Plane positions


void main()
{
    InitMain();

    gl_Position = vec4((B_position_raw_vin * 2.0f).xy, 0, 1);

    EndMain();
}
