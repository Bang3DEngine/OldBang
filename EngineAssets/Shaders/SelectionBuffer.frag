uniform vec4 selectionColor;

in vec4 B_position_raw_vout_fin;

out vec4 fragColor;
out vec4 worldPosition;

void main()
{
    fragColor = selectionColor;
    worldPosition = B_position_raw_vout_fin;
}
