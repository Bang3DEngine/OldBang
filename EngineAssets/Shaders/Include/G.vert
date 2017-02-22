#define BANG_VERTEX
#define BANG_G
#include "Uniforms.glsl"

struct B_VertexOut_FragmentIn
{
    vec4 position_world;
    vec4 normal_world;
    vec2 uv;

    vec4 position_pvm;
};

in vec3 B_position_raw_vin;
in vec3 B_normal_raw_vin;
in vec2 B_uv_raw_vin;

out vec4 B_position_world_vout_fin;
out vec4 B_normal_world_vout_fin;
out vec2 B_uv_vout_fin;

B_VertexOut_FragmentIn B_vout;

void InitMain()
{
    vec3 objectPosition = VERT_IN_POSITION_OBJECT();
    vec2 objectUv = VERT_IN_UV_OBJECT();

    // Some default values
    B_vout.position_world = B_matrix_model  * vec4(objectPosition, 1);
    B_vout.normal_world   = B_matrix_normal * vec4(VERT_IN_NORMAL_OBJECT(),   0);
    B_vout.uv             = vec2(objectUv.x, 1.0f - objectUv.y);
    B_vout.position_pvm   = B_matrix_pvm * vec4(objectPosition,   1);
}

void EndMain()
{
    // Pass the out values to the out attributes
    B_position_world_vout_fin = B_vout.position_world;
    B_normal_world_vout_fin   = B_vout.normal_world;
    B_uv_vout_fin             = B_vout.uv;
    gl_Position               = B_vout.position_pvm;
}

#include "Main.glsl"
