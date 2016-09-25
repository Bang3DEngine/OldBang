#include "PR.frag"

uniform vec4 B_tint;
uniform vec4 B_strokeColor;
uniform float B_stroke;

void Main()
{
    B_vout.color = texture2D(B_texture_0, B_vin.uv);
    B_vout.color = mix(B_vout.color, B_tint, B_tint.a);
    B_vout.color = vec4(1,1,0,1);
}
