#version 130

uniform sampler2D BANG_texture0;

in vec4 BANG_position_raw_vout_fin;
in vec4 BANG_normal_raw_vout_fin;
in vec2 BANG_uv_raw_vout_fin;

void main()
{
    vec4 color = texture2D(BANG_texture0, BANG_uv_raw_vout_fin);
    gl_FragColor = color;
}
