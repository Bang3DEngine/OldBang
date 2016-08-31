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
    B_POST_RENDER_FS_INIT_MAIN();

    // ONLY FOR EDITOR

    // SELECTION OUTLINE EFFECT
    bool isSelected = texture2D(B_materialProps_gout_fin, B_vin.uv_screen).w > 0.5;
    if (isSelected)
    {
        B_vout.color = B_vin.color + vec4(0, 0.1, 0.3, 0);
    }
    //

    //

    B_POST_RENDER_FS_END_MAIN();
}
