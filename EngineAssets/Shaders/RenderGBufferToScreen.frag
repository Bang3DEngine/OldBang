#define BANG_SP
#define BANG_FRAGMENT
#include "Uniforms.glsl"

void main()
{
    vec4 color   = texture2D(B_color_gout_fin, B_ScreenUv);
    gl_FragColor = vec4(color.rgb, 1);
}
