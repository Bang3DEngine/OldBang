B_DRAW_TO_GBUFFER_VS_DECLARE();

uniform vec3  B_world_circleCenter;
uniform float B_boundingSphereRadius;

struct B_VertexIn
{
    vec3 position_object;
    vec3 normal_object;
    vec2 uv;
};

struct B_VertexOut_FragmentIn
{
    vec4 position_world;
    vec4 normal_world;
    vec2 uv;

    vec4 position_pvm;
};

B_VertexIn             B_vin;
B_VertexOut_FragmentIn B_vout;

void main()
{
    B_DRAW_TO_GBUFFER_VS_INIT_MAIN();
    B_DRAW_TO_GBUFFER_VS_END_MAIN();
}
