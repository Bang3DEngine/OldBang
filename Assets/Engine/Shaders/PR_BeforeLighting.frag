B_POST_RENDER_FS_DECLARE();

struct B_VertexIn  // GBuffer stored properties
{
    vec3 position_world;
    vec3 normal_world;
    vec2 uv;
    vec4 diffuseColor;
    bool receivesLighting;
    float shininess;
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
    /*
      This shader just takes care of writing the
      ambientColor into the color buffer.
    */

    B_POST_RENDER_FS_INIT_MAIN();

    // TODO: Set ambient color from editor instead of hardcoded vec3(0.5f)

    if (B_vin.depth != 1.0)
    {
        vec3 ambientLight = vec3(B_vin.receivesLighting ? 0.5 : 1.0);
        B_vout.color = vec4(B_vin.diffuseColor.rgb * ambientLight, 1);
    }
    else
    {   // Background
        B_vout.color = vec4(1,1,1,1); // TODO: Use clear color
    }

    B_POST_RENDER_FS_END_MAIN();
}
