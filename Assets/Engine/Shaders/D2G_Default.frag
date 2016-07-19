B_DRAW_TO_GBUFFER_FS_DECLARE();

uniform sampler2D B_texture_0;
uniform vec4 B_material_diffuse_color;
uniform float B_renderer_receivesLighting;

in vec4 B_position_raw_vout_fin;
in vec4 B_normal_raw_vout_fin;
in vec2 B_uv_raw_vout_fin;

out vec3  B_position_fout_gin;
out vec3  B_normal_fout_gin;
out vec2  B_uv_fout_gin;
out vec3  B_diffuse_fout_gin;
out vec4  B_materialBools_fout_gin;
out float B_depth_fout_gin;

void main()
{
    B_DRAW_TO_GBUFFER_FS_INIT_MAIN();

    B_position_fout_gin = B_position_raw_vout_fin.xyz;
    B_normal_fout_gin   = B_normal_raw_vout_fin.xyz;
    B_uv_fout_gin       = B_uv_raw_vout_fin;

    float texApport = (1.0f - B_material_diffuse_color.a);
    vec3 diffTex = texture2D(B_texture_0, B_uv_raw_vout_fin).rgb * texApport;
    vec3 diffColor = B_material_diffuse_color.rgb * B_material_diffuse_color.a;
    B_diffuse_fout_gin  = diffTex + diffColor;

    B_materialBools_fout_gin.x = B_renderer_receivesLighting;

    B_depth_fout_gin    = gl_FragCoord.z;
}
