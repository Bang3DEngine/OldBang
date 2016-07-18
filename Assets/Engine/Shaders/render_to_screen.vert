#version 130

in vec3 BANG_position_raw_vin;

out vec2 BANG_uv_raw_vout_fin;

void main()
{
    BANG_uv_raw_vout_fin = (BANG_position_raw_vin + 0.5f).xy;
    gl_Position = vec4(BANG_position_raw_vin * 2.0f, 1);
}
