B_DRAW_TO_GBUFFER_FS_DECLARE();

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

    vec3 diffColor = B_material_diffuse_color.rgb;
    if (B_hasTexture > 0.5)
    {
        // ALPHA CUTTOFF
        vec4 texColor = texture2D(B_texture_0, B_vin.uv).rgba;
        if (texColor.a <= B_alphaCuttoff) discard;
        //
        // MIX DIFFUSE_COLOR AND TEXTURE_COLOR
        float texApport = (1.0 - B_material_diffuse_color.a);
        vec3  diffTex   = texColor.rgb;
        //
        B_vout.diffuseColor = diffTex * diffColor;
    }
    else
    {
        B_vout.diffuseColor = diffColor;
    }

    // ONLY FOR EDITOR SELECTION
    B_materialBools_fout_gin.w = B_gameObject_isSelected;
    //


    B_DRAW_TO_GBUFFER_FS_END_MAIN();
}
