#include "PR.frag"

void main()
{
    InitMain();

    /*
      This shader just takes care of writing the
      ambientColor into the color buffer.
    */

    if (!IsEmpty())
    {
        vec3 ambientLight = vec3(B_vin.receivesLighting ? 0.3 : 1.0);
        B_vout.color = vec4(B_vin.diffuseColor.rgb * ambientLight,
                            B_vin.diffuseColor.a);
    }

    EndMain();
}
