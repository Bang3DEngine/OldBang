#define BANG_FRAGMENT
#define BANG_G
#include "Common.glsl"
#include "Ins.glsl"

uniform vec4 selectionColor;

out vec4 fragColor;

void main()
{
    if (B_HasTexture)
    {
        vec4 texColor = texture2D(B_Texture0, B_FIn_Uv);
        if (texColor.a <= 0.0f) { discard; }
    }
    fragColor = selectionColor;
}
