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

in vec3 B_InPositionObject;
in vec3 B_InNormalObject;
in vec2 B_InUv;

out vec4 B_position_world_vout_fin;
out vec4 B_normal_world_vout_fin;
out vec2 B_uv_world_vout_fin;

B_VertexOut_FragmentIn B_vout;

void InitMain()
{
    vec3 objectPosition = B_InPositionObject;
    vec2 objectUv = B_InUv;

    // Some default values
    B_vout.position_world = B_Model  * vec4(objectPosition, 1);
    B_vout.normal_world   = B_Normal * vec4(B_InNormalObject,   0);
    B_vout.uv             = vec2(objectUv.x, 1.0f - objectUv.y);
    B_vout.position_pvm   = B_PVM * vec4(objectPosition,   1);
}

void EndMain()
{
    // Pass the out values to the out attributes
    B_position_world_vout_fin = B_vout.position_world;
    B_normal_world_vout_fin   = B_vout.normal_world;
    B_uv_world_vout_fin       = B_vout.uv;
    gl_Position               = B_vout.position_pvm;
}

#include "Main.glsl"
