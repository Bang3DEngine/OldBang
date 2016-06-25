#version 130

uniform mat4 BANG_matrix_model;

in vec3 BANG_position_raw_vin;

out vec4 BANG_position_raw_vout_fin;

void main()
{
    BANG_position_raw_vout_fin = BANG_matrix_model * vec4(BANG_position_raw_vin, 1.0);
    gl_Position = BANG_position_raw_vout_fin;
}
