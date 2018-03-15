#include "PP.frag"

uniform sampler2D B_SSAOMap;

void main()
{
    B_GIn_Color = B_SampleColor() * (1.0 - texture(B_SSAOMap, B_GetViewportUv()));
}
