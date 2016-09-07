#include "Version.glsl"

uniform vec3 selectionColor;

in vec4 B_position_raw_vout_fin;

out vec4 fragColor;
out vec4 worldPosition;

void main()
{
    fragColor = vec4(selectionColor, 1.0);
    worldPosition = B_position_raw_vout_fin;
}
