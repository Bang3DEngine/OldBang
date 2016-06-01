#version 130

uniform vec3 selectionColor;

in vec4 BANG_position_raw_vout_fin;
out vec4 fragColor;

void main()
{
    fragColor = vec4(selectionColor, 1.0);
}
