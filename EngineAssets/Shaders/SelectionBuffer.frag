#include "Uniforms.glsl"

uniform vec4 selectionColor;

in vec2 B_uv_world_vout_fin;
in vec4 B_position_raw_vout_fin;

out vec4 fragColor;
out vec4 worldPosition;

void main()
{
    if (B_HasTexture)
    {
        vec4 texColor = texture2D(B_Texture0, B_uv_world_vout_fin);
        if (texColor.a <= B_AlphaCutoff) { discard; }
    }

    fragColor = selectionColor;
    worldPosition = B_position_raw_vout_fin;
}
