#version 130

uniform sampler2D B_gout_fin_color;

in vec3 B_position_raw_vin;
in vec2 B_uv_raw_vout_fin;

void main()
{
    vec4 color   = texture2D(B_gout_fin_color, B_uv_raw_vout_fin);
    gl_FragColor = vec4(color.rgb, 1);
}
