#version 130

uniform mat4 B_matrix_model;

in vec4 B_position_raw_vout_fin;
in vec4 B_normal_raw_vout_fin;

out vec3 B_position_fout_gbufferin;
out vec3 B_normal_fout_gbufferin;
out vec2 B_uv_fout_gbufferin;
out vec3 B_diffuse_fout_gbufferin;

void main()
{
    B_position_fout_gbufferin = B_position_raw_vout_fin.xyz;
    B_normal_fout_gbufferin = B_normal_raw_vout_fin.xyz;
}
