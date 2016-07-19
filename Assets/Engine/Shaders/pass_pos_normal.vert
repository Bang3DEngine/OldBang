#version 130

in vec3 B_position_raw_vin;
in vec3 B_normal_raw_vin;

out vec4 B_position_raw_vout_fin;
out vec4 B_normal_raw_vout_fin;


void main()
{
    B_position_raw_vout_fin = B_position_raw_vin;
    B_normal_raw_vout_fin = B_normal_raw_vin;

    gl_Position = B_position_raw_vout_fin;
}
