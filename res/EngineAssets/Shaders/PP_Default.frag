#include "PP.frag"

void Main()
{
    vec4 screenColor = B_SampleColor();
    screenColor = vec4(1.0 - screenColor.rgb, 1.0);
    B_FOut.Color = screenColor;
}
