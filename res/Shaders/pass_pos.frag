#version 130

in vec4 BANG_position_raw_vout_fin;

void main()
{
    gl_FragColor = vec4(abs(BANG_position_raw_vout_fin.xyz), 1);
}
