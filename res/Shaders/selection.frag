#version 130

uniform vec4 selectionColor;

in vec4 BANG_position_raw_vout_fin;
out vec4 fragColor;

void main()
{
    fragColor = selectionColor;
}
