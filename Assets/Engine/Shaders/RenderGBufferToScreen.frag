#include "Version.glsl"
#define BANG_FRAGMENT
#include "Uniforms.glsl"

uniform sampler2D B_color_gout_fin;

void main()
{
    vec4 color   = texture2D(B_color_gout_fin, B_screen_coord_norm);
    gl_FragColor = vec4(color.rgb, 1);
}
