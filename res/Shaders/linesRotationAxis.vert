#version 130

uniform mat4 BANG_matrix_pvm;
uniform mat4 BANG_matrix_model;
uniform vec3 wCircleCenter;

in vec3 BANG_position_raw_vin;

out vec3 wPos;
out vec4 BANG_position_raw_vout_fin;

void main()
{
    BANG_position_raw_vout_fin = vec4(BANG_position_raw_vin, 1);
    gl_Position = BANG_matrix_pvm * vec4(BANG_position_raw_vin, 1);
    wPos = (BANG_matrix_model * vec4(BANG_position_raw_vin, 1)).xyz;
}
