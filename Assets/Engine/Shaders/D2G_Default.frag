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
    vec4 diffuseColor;
    bool receivesLighting;
    float shininess;
    float depth;
};

B_VertexIn B_vin;
B_VertexOut_GBufferIn B_vout;

void main()
{
    B_DRAW_TO_GBUFFER_FS_INIT_MAIN();

    vec4 diffColor = B_material_diffuse_color;
    if (B_hasTexture > 0.5)
    {
        // ALPHA CUTTOFF
        vec4 texColor = texture2D(B_texture_0, B_vin.uv);
        if (texColor.a <= B_alphaCuttoff) discard;
        //

        // MIX DIFFUSE_COLOR AND TEXTURE_COLOR
        B_vout.diffuseColor = texColor * diffColor;
        //
    }
    else
    {
        B_vout.diffuseColor = diffColor;
    }

    B_DRAW_TO_GBUFFER_FS_END_MAIN();
}
