#version 130

uniform mat4 BANG_matrix_model;

in vec4 BANG_position_vout_fin;

void main()
{
    gl_FragColor = vec4(BANG_position_vout_fin.xyz, 1);
}
