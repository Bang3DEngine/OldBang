#version 130

uniform mat4 B_matrix_pvm;
uniform mat4 B_matrix_model;
uniform vec3 wCircleCenter;

in vec3 B_position_raw_vin;

out vec3 wPos;
out vec4 B_position_raw_vout_fin;

void main()
{
    B_position_raw_vout_fin = vec4(B_position_raw_vin, 1);
    gl_Position = B_matrix_pvm * vec4(B_position_raw_vin, 1);
    wPos = (B_matrix_model * vec4(B_position_raw_vin, 1)).xyz;
}
