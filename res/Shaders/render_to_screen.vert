#version 130

in vec3 BANG_position_raw_vin;

out vec3 BANG_position_raw_vout_fin;

void main()
{
    BANG_position_raw_vout_fin = BANG_position_raw_vin * 2.0f;
    gl_Position = vec4(BANG_position_raw_vin * 2.0f, 1);
}
