#version 130

uniform sampler2D BANG_texture_0; //positions
uniform sampler2D BANG_texture_1; //normals
uniform sampler2D BANG_texture_2; //uvs
uniform sampler2D BANG_texture_3; //diffuse
uniform sampler2D BANG_texture_4; //depth

in vec2 BANG_uv_raw_vout_fin;

void main()
{
    vec3  BANG_position = texture2D(BANG_texture_0, BANG_uv_raw_vout_fin).xyz;
    vec3  BANG_normal   = normalize(texture2D(BANG_texture_1, BANG_uv_raw_vout_fin).xyz);
    vec2  BANG_uv       = texture2D(BANG_texture_2, BANG_uv_raw_vout_fin).xy;
    vec3  BANG_diffuse  = texture2D(BANG_texture_3, BANG_uv_raw_vout_fin).rgb;
    float BANG_depth    = texture2D(BANG_texture_4, BANG_uv_raw_vout_fin).x ;

    float lightDot = dot(BANG_normal, normalize(vec3(1, 1, 1)));
    gl_FragColor = vec4(BANG_diffuse * lightDot, 1);
}
