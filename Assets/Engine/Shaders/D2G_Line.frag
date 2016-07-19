B_DRAW_TO_GBUFFER_FS_DECLARE();

uniform vec4 B_material_diffuse_color;
uniform float B_renderer_receivesLighting;

in vec4 B_positionRaw_vout_fin;

out vec3  B_position_fout_gin;
out vec3  B_normal_fout_gin;
out vec2  B_uv_fout_gin;
out vec3  B_diffuse_fout_gin;
out vec4  B_materialBools_fout_gin;
out float B_depth_fout_gin;

void main()
{
    B_DRAW_TO_GBUFFER_FS_INIT_MAIN();

    B_position_fout_gin = B_positionRaw_vout_fin.xyz;
    B_normal_fout_gin = vec3(0.0f);
    B_uv_fout_gin = vec2(0.0f);
    B_diffuse_fout_gin = B_material_diffuse_color.rgb;

    B_materialBools_fout_gin.x = B_renderer_receivesLighting;

    B_depth_fout_gin = gl_FragCoord.z;
}
