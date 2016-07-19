#version 130

uniform sampler2D B_texture_0; //positions
uniform sampler2D B_texture_1; //normals
uniform sampler2D B_texture_2; //uvs
uniform sampler2D B_texture_3; //diffuse
uniform sampler2D B_texture_4; //depth

in vec2 B_uv_raw_vout_fin;

void main()
{
    vec3  B_position = texture2D(B_texture_0, B_uv_raw_vout_fin).xyz;
    vec3  B_normal   = texture2D(B_texture_1, B_uv_raw_vout_fin).xyz;
    vec2  B_uv       = texture2D(B_texture_2, B_uv_raw_vout_fin).xy;
    vec3  B_diffuse  = texture2D(B_texture_3, B_uv_raw_vout_fin).rgb;
    float B_depth    = texture2D(B_texture_4, B_uv_raw_vout_fin).x ;

    gl_FragColor = vec4(B_diffuse, 1);
}
