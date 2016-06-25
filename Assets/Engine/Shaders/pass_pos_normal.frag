#version 130

uniform mat4 BANG_matrix_model;

in vec4 BANG_position_raw_vout_fin;
in vec4 BANG_normal_raw_vout_fin;

out vec3 BANG_position_fout_gbufferin;
out vec3 BANG_normal_fout_gbufferin;
out vec2 BANG_uv_fout_gbufferin;
out vec3 BANG_diffuse_fout_gbufferin;

void main()
{
    BANG_position_fout_gbufferin = BANG_position_raw_vout_fin.xyz;
    BANG_normal_fout_gbufferin = BANG_normal_raw_vout_fin.xyz;
}
