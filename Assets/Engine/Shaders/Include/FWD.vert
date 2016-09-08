#include "Version.glsl"
#define BANG_VERTEX
#include "Uniforms.glsl"

struct B_VertexIn
{
    vec3 position_object;
    vec3 normal_object;
    vec2 uv;
};

struct B_VertexOut_FragmentIn
{
    vec3 position_world;
    vec3 normal_world;
    vec2 uv;

    vec4 position_pvm;
};

in vec3 B_position_raw_vin;
in vec3 B_normal_raw_vin;
in vec2 B_uv_raw_vin;

out vec3 B_position_world_vout_fin;
out vec3 B_normal_world_vout_fin;
out vec2 B_uv_vout_fin;

B_VertexIn             B_vin;
B_VertexOut_FragmentIn B_vout;

void InitMain()
{
    // Fill vin values
    B_vin.position_object = B_position_raw_vin;
    B_vin.normal_object   = B_normal_raw_vin;
    B_vin.uv              = B_uv_raw_vin;

    // Some default values
    B_vout.position_world = (B_matrix_model  * vec4(B_vin.position_object, 1)).xyz;
    B_vout.normal_world   = (B_matrix_normal * vec4(B_vin.normal_object,   0)).xyz;
    B_vout.uv             = vec2(B_vin.uv.x, 1.0f - B_vin.uv.y);
    B_vout.position_pvm   = B_matrix_pvm * vec4(B_vin.position_object,   1);
}

void EndMain()
{
    // Pass the out values to the out attributes
    B_position_world_vout_fin = B_vout.position_world;
    B_normal_world_vout_fin   = B_vout.normal_world;
    B_uv_vout_fin             = B_vout.uv;
    gl_Position               = B_vout.position_pvm;
}
