#include "PR.frag"

void main()
{
    InitMain();

    /*
      This shader just takes care of writing the
      ambientColor into the color buffer.
    */


    // TODO: Set ambient color from editor instead of hardcoded vec3(0.5f)

    if (B_vin.depth != 1.0)
    {
        vec3 ambientLight = vec3(B_vin.receivesLighting ? 0.3 : 1.0);
        B_vout.color = vec4(B_vin.diffuseColor.rgb * ambientLight,
                            B_vin.diffuseColor.a);
    }
    else
    {   // Background
        B_vout.color = vec4(1,1,1,1); // TODO: Use clear color
    }

    EndMain();
}
