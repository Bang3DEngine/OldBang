#version 130

uniform mat4 BANG_matrix_model;
uniform sampler2D BANG_tex0;

in vec4 BANG_position_raw_vout_fin;
in vec4 BANG_normal_raw_vout_fin;
in vec2 BANG_uv_raw_vout_fin;

void main()
{
    vec4 color = texture2D(BANG_tex0, BANG_uv_raw_vout_fin);
    gl_FragColor = color;
}
