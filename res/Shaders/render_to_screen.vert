#version 130

in vec3 BANG_position_raw_vin;

out vec3 BANG_uv_raw_vout_fin;

void main()
{
    BANG_uv_raw_vout_fin = BANG_position_raw_vin * 0.5 + vec3(0.5);
    gl_Position = vec4(BANG_position_raw_vin, 1);
}
