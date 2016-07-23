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

    // ONLY FOR EDITOR

    // SELECTION OUTLINE EFFECT
    float isSelected = texture2D(B_materialBools_gout_fin, B_vin.uv_screen).w;
    int stop = 0;
    if(isSelected < 0.5f)
    {
        int stroke = 1;
        for(int s = 0; s <= stroke && stop == 0; ++s)
        {
            for(int x = -s; x <= s && stop == 0; ++x)
            {
                for(int y = -s; y <= s && stop == 0; ++y)
                {
                    vec2 offset = vec2(B_pixel_step.x * x, B_pixel_step.y * y);
                    float sel = texture2D(B_materialBools_gout_fin, B_vin.uv_screen + offset).w;
                    if (sel > 0.5f)
                    {
                        // B_vout.color = vec4((vec3(1) - B_vin.color.rgb), 1);
                        B_vout.color = vec4(vec3(0), 1);
                        stop = 1;
                    }
                }
            }
        }
    }
    else
    {
        B_vout.color += vec4(0, 0.1, 0.3, 0);
    }
    //

    //

    B_POST_RENDER_FS_END_MAIN();
}
