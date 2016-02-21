#version 130

uniform sampler2D BANG_texture_0;
uniform vec4 BANG_material_diffuse_color;

in vec4 BANG_position_raw_vout_fin;
in vec4 BANG_normal_raw_vout_fin;
in vec2 BANG_uv_raw_vout_fin;

out vec3 BANG_position_fout_gbufferin;
out vec3 BANG_normal_fout_gbufferin;
out vec2 BANG_uv_fout_gbufferin;
out vec3 BANG_diffuse_fout_gbufferin;
out float BANG_depth_fout_gbufferin;


void main()
{
    BANG_position_fout_gbufferin = BANG_position_raw_vout_fin.xyz;
    BANG_normal_fout_gbufferin   = BANG_normal_raw_vout_fin.xyz;
    BANG_uv_fout_gbufferin       = BANG_uv_raw_vout_fin;

    float texApport = (1.0f - BANG_material_diffuse_color.a);
    BANG_diffuse_fout_gbufferin  = texture2D(BANG_texture_0, BANG_uv_raw_vout_fin).rgb * texApport +
                                   BANG_material_diffuse_color.rgb * BANG_material_diffuse_color.a;

    BANG_depth_fout_gbufferin    = gl_FragCoord.z;
}
