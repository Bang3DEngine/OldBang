#include "PR.frag"

uniform float B_image_left, B_image_up,
              B_image_right, B_image_bot;
uniform vec4 B_tint;
uniform vec4 B_strokeColor;
uniform float B_stroke;

void Main()
{
    vec2 uvScreen = B_vin.uv_screen * 2.0f - 1.0f;
    if (uvScreen.x <= B_image_left  + B_stroke                         ||
        uvScreen.x >= B_image_right - B_stroke                         ||
        uvScreen.y >= B_image_up    - B_stroke * B_screen_aspect_ratio ||
        uvScreen.y <= B_image_bot   + B_stroke * B_screen_aspect_ratio)
    {
        B_vout.color = B_strokeColor;
    }
    else
    {
        B_vout.color = texture2D(B_texture_0, B_vin.uv);
        B_vout.color = mix(B_vout.color, B_tint, B_tint.a);
    }

  //  B_vout.color = vec4(B_vin.uv_screen, 0, 1);
}
