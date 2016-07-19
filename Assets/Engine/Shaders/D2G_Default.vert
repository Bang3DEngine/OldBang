#version 130

B_DRAW_TO_GBUFFER_VS_DECLARE();

uniform mat4 B_matrix_model;
uniform mat4 B_matrix_view;
uniform mat4 B_matrix_projection;
uniform mat4 B_matrix_pvm;

in vec3 B_position_raw_vin;
in vec3 B_normal_raw_vin;
in vec2 B_uv_raw_vin;

out vec4 B_position_raw_vout_fin;
out vec4 B_normal_raw_vout_fin;
out vec2 B_uv_raw_vout_fin;

void main()
{
    B_DRAW_TO_GBUFFER_VS_INIT_MAIN();

    B_position_raw_vout_fin = B_matrix_model * vec4(B_position_raw_vin, 1.0);
    B_normal_raw_vout_fin   = B_matrix_model * vec4(B_normal_raw_vin, 0.0);
    B_uv_raw_vout_fin       = vec2(B_uv_raw_vin.x, 1.0f - B_uv_raw_vin.y);

    gl_Position = B_matrix_pvm * vec4(B_position_raw_vin, 1.0);
}
