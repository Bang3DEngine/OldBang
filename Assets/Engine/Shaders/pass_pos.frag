#version 130

in vec4 B_position_raw_vout_fin;

out vec3 B_position_fout_gbufferin;

void main()
{
    B_position_fout_gbufferin = B_position_raw_vout_fin.xyz;
}
