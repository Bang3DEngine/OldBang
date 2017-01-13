#include "PR.frag"

uniform vec2 B_rectMinCoord = vec2(-1, -1);
uniform vec2 B_rectMaxCoord = vec2( 1,  1);

void Main()
{
    /*
      This shader just takes care of writing the
      ambientColor into the color buffer.
    */

    float ambientLight = (FRAG_IN_RECEIVES_LIGHTING() ? 0.1 : 1.0);
    vec4 diffColor = FRAG_IN_DIFFUSE_COLOR();
    B_vout.color = vec4(diffColor.rgb * ambientLight, diffColor.a);
}
