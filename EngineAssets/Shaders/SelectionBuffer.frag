#include "Common.glsl"

uniform vec4 selectionColor;

in vec2 B_FragIn_Uv;
in vec4 B_FragIn_PositionWorld;

out vec4 fragColor;
out vec4 worldPosition;

void main()
{
    if (B_HasTexture)
    {
        vec4 texColor = texture2D(B_Texture0, B_FragIn_Uv);
        if (texColor.a <= B_AlphaCutoff) { discard; }
    }

    fragColor = selectionColor;
    worldPosition = B_FragIn_PositionWorld;
}
