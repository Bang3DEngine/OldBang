#version 130

uniform sampler2D BANG_texture_0;

in vec3 BANG_position_raw_vout_fin;

void main()
{
    if(BANG_position_raw_vout_fin.x < -0.9) gl_FragColor = vec4(0, vec3(1));
    else gl_FragColor = texture2D(BANG_texture_0, BANG_position_raw_vout_fin.xy);
}
