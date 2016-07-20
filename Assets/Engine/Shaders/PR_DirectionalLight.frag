B_POST_RENDER_FS_DECLARE();

struct B_VertexIn  // GBuffer stored properties
{
    vec3 position_world;
    vec3 normal_world;
    vec2 uv;
    vec3 diffuseColor;
    float receivesLighting;
    float depth;

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

    if(B_vin.receivesLighting > 0.5f)
    {
        float lightDot = dot(B_vin.normal_world, normalize(vec3(1, 1, 1)));
        B_vout.color = vec4(B_vin.diffuseColor * lightDot, 1);
    }
    else
    {
        B_vout.color = vec4(B_vin.diffuseColor, 1);
    }

    B_POST_RENDER_FS_END_MAIN();
}
