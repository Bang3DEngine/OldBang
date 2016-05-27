#version 130

uniform vec4 selectionColor;

out vec4 fragColor;

void main()
{
    fragColor = selectionColor;
}
