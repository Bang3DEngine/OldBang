B_DRAW_TO_GBUFFER_FS_DECLARE();

uniform vec3  B_world_circleCenter;
uniform float B_boundingSphereRadius;

struct B_VertexIn
{
    vec4 position_world;
    vec4 normal_world;
    vec2 uv;
};

struct B_VertexOut_GBufferIn   // GBuffer
{
    vec3 position_world;
    vec3 normal_world;
    vec2 uv;
    vec3 diffuseColor;
    float receivesLighting;
    float depth;
};

B_VertexIn B_vin;
B_VertexOut_GBufferIn B_vout;

void main()
{
    B_DRAW_TO_GBUFFER_FS_INIT_MAIN();

    vec3 world_cameraUp     = normalize( (B_matrix_view * vec4(0,1,0,0)).xyz );
    vec3 world_maxDistPoint = B_world_circleCenter + world_cameraUp * B_boundingSphereRadius;

    float maxDist = distance(B_position_camera, world_maxDistPoint);
    float dToV    = distance(B_position_camera, B_position_world_vout_fin.xyz);

    if (dToV > maxDist) discard;

    B_DRAW_TO_GBUFFER_FS_END_MAIN();
}
