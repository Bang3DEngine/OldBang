#include "PP.frag"

void main()
{
    vec4 col = B_SampleColor();
    B_GIn_Color = vec4( vec3(1.0) - col.rgb, 1 );
}
