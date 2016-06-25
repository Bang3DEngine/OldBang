#version 130

uniform vec3 selectionColor;

out vec4 fragColor;

void main()
{
    fragColor = vec4(selectionColor, 1.0);
}
