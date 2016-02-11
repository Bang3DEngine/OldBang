#version 130

uniform sampler2D BANG_texture_0;

in vec3 BANG_uv_raw_vout_fin;

void main()
{
    gl_FragColor = vec4(BANG_uv_raw_vout_fin.xy, 0, 1) + 0.3 * texture2D(BANG_texture_0, BANG_uv_raw_vout_fin.xy);
}
