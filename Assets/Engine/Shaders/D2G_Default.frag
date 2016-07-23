B_DRAW_TO_GBUFFER_FS_DECLARE();

uniform sampler2D B_texture_0;

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

    float texApport = (1.0f - B_material_diffuse_color.a);
    vec3  diffTex   = texture2D(B_texture_0, B_vin.uv).rgb * texApport;
    vec3  diffColor = B_material_diffuse_color.rgb * B_material_diffuse_color.a;
    B_vout.diffuseColor  = diffTex + diffColor;

    // ONLY FOR EDITOR
    B_materialBools_fout_gin.w = B_gameObject_isSelected;
    /*
    if(B_gameObject_isSelected > 0.5f)
    {
        B_materialBools_fout_gin.w = B_gameObject_isSelected;
        B_vout.diffuseColor  = vec3(1,1,0);
    }
    */
    //

    B_DRAW_TO_GBUFFER_FS_END_MAIN();
}
