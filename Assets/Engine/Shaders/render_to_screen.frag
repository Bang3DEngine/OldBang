#version 130

uniform sampler2D BANG_gout_fin_position; //positions
uniform sampler2D BANG_gout_fin_normal; //normals
uniform sampler2D BANG_gout_fin_uv; //uvs
uniform sampler2D BANG_gout_fin_diffuse; //diffuse
uniform sampler2D BANG_gout_fin_depth; //depth

in vec2 BANG_uv_raw_vout_fin;

void main()
{
    vec3  BANG_position = texture2D(BANG_gout_fin_position, BANG_uv_raw_vout_fin).xyz;
    vec3  BANG_normal   = normalize(texture2D(BANG_gout_fin_normal, BANG_uv_raw_vout_fin).xyz);
    vec2  BANG_uv       = texture2D(BANG_gout_fin_uv, BANG_uv_raw_vout_fin).xy;
    vec3  BANG_diffuse  = texture2D(BANG_gout_fin_diffuse, BANG_uv_raw_vout_fin).rgb;
    float BANG_depth    = texture2D(BANG_gout_fin_depth, BANG_uv_raw_vout_fin).x;

    gl_FragColor = vec4(BANG_diffuse, 1);
}
