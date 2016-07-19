B_DRAW_TO_GBUFFER_FS_DECLARE();

uniform mat4 B_matrix_view;
uniform vec3 B_position_camera;
uniform vec4 B_material_diffuse_color;
uniform vec3 B_world_circleCenter;
uniform float B_boundingSphereRadius;
uniform float B_renderer_receivesLighting;

in vec3 B_world_posisition;
in vec4 B_position_raw_vout_fin;

out vec3 B_position_fout_gin;
out vec3 B_normal_fout_gin;
out vec2 B_uv_fout_gin;
out vec3 B_diffuse_fout_gin;
out vec3 B_materialBools_fout_gin;
out float B_depth_fout_gin;

void main()
{
    B_DRAW_TO_GBUFFER_FS_INIT_MAIN();

    vec3 world_cameraUp = normalize( (B_matrix_view * vec4(0,1,0,0)).xyz );
    vec3 world_maxDistPoint = B_world_circleCenter + B_world_cameraUp * boundingSphereRadius;

    float maxDist = distance(B_position_camera, B_world_maxDistPoint);
    float dToV = distance(B_position_camera, B_world_position);

    if(dToV > maxDist) discard;

    B_position_fout_gin = B_position_raw_vout_fin.xyz;
    B_normal_fout_gin = vec3(0.0f);
    B_uv_fout_gin = vec2(0.0f);
    B_diffuse_fout_gin = B_material_diffuse_color.rgb;

    B_materialBools_fout_gin.x = B_renderer_receivesLighting;

    B_depth_fout_gin = gl_FragCoord.z;
}
