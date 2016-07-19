B_DRAW_TO_GBUFFER_VS_DECLARE();

uniform mat4 B_matrix_pvm;
uniform mat4 B_matrix_model;
uniform vec3 B_world_circleCenter;

in vec3 B_position_raw_vin;

out vec3 B_world_position;
out vec4 B_position_raw_vout_fin;

void main()
{
    B_DRAW_TO_GBUFFER_VS_INIT_MAIN();

    B_position_raw_vout_fin = vec4(B_position_raw_vin, 1);
    gl_Position = B_matrix_pvm * vec4(B_position_raw_vin, 1);
    B_world_position = (B_matrix_model * vec4(B_position_raw_vin, 1)).xyz;
}
