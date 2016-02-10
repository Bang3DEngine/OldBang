#version 130

uniform mat4 BANG_matrix_model;
uniform mat4 BANG_matrix_view;
uniform mat4 BANG_matrix_projection;
uniform mat4 BANG_matrix_pvm;

in vec3 BANG_position_raw_vin;
in vec3 BANG_normal_raw_vin;
in vec2 BANG_uv_raw_vin;

out vec4 BANG_position_raw_vout_fin;
out vec4 BANG_normal_raw_vout_fin;
out vec2 BANG_uv_raw_vout_fin;

void main()
{
    BANG_position_raw_vout_fin = BANG_matrix_model * vec4(BANG_position_raw_vin, 1.0);
    BANG_normal_raw_vout_fin = BANG_matrix_model * vec4(BANG_normal_raw_vin, 0.0);
    BANG_uv_raw_vout_fin = BANG_uv_raw_vin;

    gl_Position = BANG_matrix_pvm * vec4(BANG_position_raw_vin, 1.0);
}
