#include "PR.vert"

uniform vec2 B_rectMinCoord = vec2(-1, -1);
uniform vec2 B_rectMaxCoord = vec2( 1,  1);

in vec3 B_position_raw_vin; // Plane positions

void Main()
{
    vec2 rectSize = B_rectMaxCoord - B_rectMinCoord;
    vec2 rectCenter =  (B_rectMaxCoord + B_rectMinCoord) * 0.5;
    gl_Position = vec4( (B_position_raw_vin * 2.0f).xy *
                        rectSize  * 0.5 + rectCenter,
                        0, 1);
}
