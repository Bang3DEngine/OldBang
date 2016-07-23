B_POST_RENDER_FS_DECLARE();

struct B_VertexIn  // GBuffer stored properties
{
    vec3 position_world;
    vec3 normal_world;
    vec2 uv;
    vec3 diffuseColor;
    float receivesLighting;
    float depth;
    vec4 color;

    vec2 uv_screen;
};

struct B_VertexOut
{
    vec4 color;
};

B_VertexIn B_vin;
B_VertexOut B_vout;

void main()
{
    B_POST_RENDER_FS_INIT_MAIN();

    // HERE YOUR CODE

    B_POST_RENDER_FS_END_MAIN();
}
