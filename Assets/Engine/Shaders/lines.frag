#version 130

uniform vec4 BANG_material_diffuse_color;
uniform float BANG_renderer_receivesLighting;

in vec4 BANG_position_raw_vout_fin;

out vec3  BANG_position_fout_gbufferin;
out vec3  BANG_normal_fout_gbufferin;
out vec2  BANG_uv_fout_gbufferin;
out vec3  BANG_diffuse_fout_gbufferin;
out vec4  BANG_material_bools_fout_gbufferin;
out float BANG_depth_fout_gbufferin;

void main()
{
    BANG_position_fout_gbufferin = BANG_position_raw_vout_fin.xyz;
    BANG_normal_fout_gbufferin = vec3(0.0f);
    BANG_uv_fout_gbufferin = vec2(0.0f);
    BANG_diffuse_fout_gbufferin = BANG_material_diffuse_color.rgb;

    BANG_material_bools_fout_gbufferin.x = BANG_renderer_receivesLighting;

    BANG_depth_fout_gbufferin = gl_FragCoord.z;
}
