#version 130

in vec3 BANG_position_vin;

out vec4 BANG_position_vout_fin;


void main()
{
    BANG_position_vout_fin = BANG_position_vin;
    gl_Position = BANG_position_vout_fin;
}
