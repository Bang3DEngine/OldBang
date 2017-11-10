#include "PP.frag"

void Main()
{
    vec4 col = B_SampleColor();
    B_FOut.Color = vec4( vec3(1.0) - col.rgb, 1 );
}
