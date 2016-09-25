#include "PR.frag"

void Main()
{
    /*
      This shader just takes care of writing the
      ambientColor into the color buffer.
    */

    float ambientLight = (B_vin.receivesLighting ? 0.3 : 1.0);
    B_vout.color = vec4(B_vin.diffuseColor.rgb * ambientLight,
                        B_vin.diffuseColor.a);
}
