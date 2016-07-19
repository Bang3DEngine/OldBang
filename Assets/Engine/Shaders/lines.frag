#version 130

uniform vec4 B_material_diffuse_color;
uniform float B_renderer_receivesLighting;

in vec4 B_position_raw_vout_fin;

out vec3  B_position_fout_gbufferin;
out vec3  B_normal_fout_gbufferin;
out vec2  B_uv_fout_gbufferin;
out vec3  B_diffuse_fout_gbufferin;
out vec4  B_material_bools_fout_gbufferin;
out float B_depth_fout_gbufferin;

void main()
{
    B_position_fout_gbufferin = B_position_raw_vout_fin.xyz;
    B_normal_fout_gbufferin = vec3(0.0f);
    B_uv_fout_gbufferin = vec2(0.0f);
    B_diffuse_fout_gbufferin = B_material_diffuse_color.rgb;

    B_material_bools_fout_gbufferin.x = B_renderer_receivesLighting;

    B_depth_fout_gbufferin = gl_FragCoord.z;
}
