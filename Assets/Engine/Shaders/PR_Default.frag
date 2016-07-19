#version 130

uniform sampler2D B_gout_fin_position; //positions
uniform sampler2D B_gout_fin_normal; //normals
uniform sampler2D B_gout_fin_uv; //uvs
uniform sampler2D B_gout_fin_diffuse; //diffuse
uniform sampler2D B_gout_fin_material_bools; //materialBools (receivesLighting, -, -, -)
uniform sampler2D B_gout_fin_depth; //depth

in vec2 B_uv_raw_vout_fin;

void main()
{
    vec2 suvs = B_uv_raw_vout_fin;

    vec3  B_position         = texture2D(B_gout_fin_position,         suvs).xyz;
    vec3  B_normal           = texture2D(B_gout_fin_normal,           suvs).xyz;
    vec2  B_uv               = texture2D(B_gout_fin_uv,               suvs).xy;
    vec3  B_diffuse          = texture2D(B_gout_fin_diffuse,          suvs).rgb;
    float B_receivesLighting = texture2D(B_gout_fin_material_bools,   suvs).x;
    float B_depth            = texture2D(B_gout_fin_depth,            suvs).x;

    B_normal = normalize(B_normal);

    gl_FragColor = vec4(B_diffuse, 1);
}
