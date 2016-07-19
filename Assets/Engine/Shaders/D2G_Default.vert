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

struct B_VertexIn
{
    vec3 position_object;
    vec3 normal_object;
    vec2 uv_object;
};

struct B_VertexOut_FragmentIn
{
    vec4 position_world;
    vec4 normal_world;
    vec2 uv_object;

    vec4 position_pvm;
};

B_VertexIn             B_vin;
B_VertexOut_FragmentIn B_vout;

void main()
{
    B_DRAW_TO_GBUFFER_VS_INIT_MAIN();

    B_vin.position_object = B_position_raw_vin;
    B_vin.normal_object   = B_normal_raw_vin;
    B_vin.uv_object       = B_uv_raw_vin;

    B_vout.position_world = B_matrix_model * vec4(B_vin.position_object, 1.0);
    B_vout.normal_world   = B_matrix_model * vec4(B_vin.normal_object,   0.0);
    B_vout.uv_object      = vec2(B_vin.uv_object.x, 1.0f - B_vin.uv_object.y);
    B_vout.position_pvm   = B_matrix_pvm * vec4(B_vin.position_object,   1.0);

    //B_position_raw_vout_fin = B_matrix_model * vec4(B_position_raw_vin, 1.0);
    //B_normal_raw_vout_fin   = B_matrix_model * vec4(B_normal_raw_vin, 0.0);
    //B_uv_raw_vout_fin       = vec2(B_uv_raw_vin.x, 1.0f - B_uv_raw_vin.y);

    B_position_raw_vout_fin = B_vout.position_world;
    B_normal_raw_vout_fin   = B_vout.normal_world;
    B_uv_raw_vout_fin       = B_vout.uv_object;
    gl_Position = B_matrix_pvm * vec4(B_vin.position_object, 1.0);
}
