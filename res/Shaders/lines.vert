#version 130

uniform mat4 BANG_matrix_pvm;
in vec3 BANG_position_raw_vin;

out vec4 BANG_position_raw_vout_fin;

void main()
{
    BANG_position_raw_vout_fin = vec4(BANG_position_raw_vin, 1.0);
    gl_Position = BANG_matrix_pvm * vec4(BANG_position_raw_vin, 1.0);
}
