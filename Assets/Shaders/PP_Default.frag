#include "PP.frag"

void main()
{
    vec4 screenColor = B_SampleColor();
    screenColor = vec4(1.0 - screenColor.rgb, 1.0);
    B_GIn_Color = screenColor;
}
