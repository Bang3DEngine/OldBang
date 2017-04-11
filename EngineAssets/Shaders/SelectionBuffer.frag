#include "Common.glsl"

uniform vec4 selectionColor;

in vec4 B_FragIn_PositionWorld;
in vec3 B_FragIn_NormalWorld;
in vec2 B_FragIn_Uv;

out vec4 fragColor;
out vec4 worldPosition;

void main()
{
    if (B_HasTexture)
    {
        vec4 texColor = texture2D(B_Texture0, B_FragIn_Uv);
        if (texColor.a <= 0.0f) { discard; }
    }

    fragColor = selectionColor;
    worldPosition = B_FragIn_PositionWorld;
}
