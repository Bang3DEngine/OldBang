#version 130

in vec3 BANG_position_raw_vin;

out vec4 BANG_position_raw_vout_fin;

void main()
{
    BANG_position_raw_vout_fin = BANG_position_raw_vin;
    gl_Position = BANG_position_raw_vout_fin;
}
