#version 130

uniform sampler2D BANG_texture_0;

in vec4 BANG_position_raw_vout_fin;
in vec4 BANG_normal_raw_vout_fin;
in vec2 BANG_uv_raw_vout_fin;

out vec3 BANG_position_fout;

void main()
{
    vec4 color = texture2D(BANG_texture_0, BANG_uv_raw_vout_fin);
    BANG_position_fout = color.rgb;
}
