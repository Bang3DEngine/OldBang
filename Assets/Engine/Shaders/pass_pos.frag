#version 130

in vec4 BANG_position_raw_vout_fin;

out vec3 BANG_position_fout_gbufferin;

void main()
{
    BANG_position_fout_gbufferin = BANG_position_raw_vout_fin.xyz;
}
