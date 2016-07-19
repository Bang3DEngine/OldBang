#version 130

uniform mat4 B_matrix_model;

in vec3 B_position_raw_vin;

out vec4 B_position_raw_vout_fin;

void main()
{
    B_position_raw_vout_fin = B_matrix_model * vec4(B_position_raw_vin, 1.0);
    gl_Position = B_position_raw_vout_fin;
}
