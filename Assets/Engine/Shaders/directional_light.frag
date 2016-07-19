#version 130

uniform sampler2D BANG_gout_fin_position; //positions
uniform sampler2D BANG_gout_fin_normal; //normals
uniform sampler2D BANG_gout_fin_uv; //uvs
uniform sampler2D BANG_gout_fin_diffuse; //diffuse
uniform sampler2D BANG_gout_fin_material_bools; //materialBools (receivesLighting, -, -, -)
uniform sampler2D BANG_gout_fin_depth; //depth

in vec2 BANG_uv_raw_vout_fin;

void main()
{
    vec2 suvs = BANG_uv_raw_vout_fin;

    vec3  BANG_position         = texture2D(BANG_gout_fin_position,         suvs).xyz;
    vec3  BANG_normal           = texture2D(BANG_gout_fin_normal,           suvs).xyz;
    vec2  BANG_uv               = texture2D(BANG_gout_fin_uv,               suvs).xy;
    vec3  BANG_diffuse          = texture2D(BANG_gout_fin_diffuse,          suvs).rgb;
    float BANG_receivesLighting = texture2D(BANG_gout_fin_material_bools,   suvs).x;
    float BANG_depth            = texture2D(BANG_gout_fin_depth,            suvs).x;

    BANG_normal = normalize(BANG_normal);

    if(BANG_receivesLighting > 0.5f)
    {
        float lightDot = dot(BANG_normal, normalize(vec3(1, 1, 1)));
        gl_FragColor = vec4(BANG_diffuse * lightDot, 1);
    }
    else
    {
        gl_FragColor = vec4(BANG_diffuse, 1);
    }
}
