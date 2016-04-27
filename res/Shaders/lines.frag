#version 130

uniform vec4 BANG_line_color;

in vec4 BANG_position_raw_vout_fin;

out vec3 BANG_position_fout_gbufferin;
out vec3 BANG_diffuse_fout_gbufferin;
out float BANG_depth_fout_gbufferin;

void main()
{
    BANG_position_fout_gbufferin = BANG_position_raw_vout_fin.xyz;
    BANG_diffuse_fout_gbufferin = BANG_line_color;
    BANG_depth_fout_gbufferin = gl_FragCoord.z;
}
