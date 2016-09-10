#include "Version.glsl"

uniform sampler2D B_color_gout_fin;

in vec2 B_screen_uv_vout_fin;

void main()
{
    vec4 color   = texture2D(B_color_gout_fin, B_screen_uv_vout_fin);
    gl_FragColor = vec4(color.rgb, 1);
}
