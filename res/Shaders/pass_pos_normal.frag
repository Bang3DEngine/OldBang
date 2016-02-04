#version 130

uniform mat4 BANG_matrix_model;

in vec4 BANG_position_raw_vout_fin;
in vec4 BANG_normal_raw_vout_fin;

void main()
{
    gl_FragColor = vec4(BANG_normal_raw_vout_fin.xyz * 0.5 + vec3(0.5), 1);
}
