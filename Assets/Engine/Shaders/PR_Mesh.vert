#include "PR.vert"

/*
  This shader just passes the screen-uvs of a Mesh to the PR**.frag
  It's like passing the screen uvs to do a deferred PR effect,
  but instead we do this for a mesh (less rendering).

  This is useful for:
    - PointLight rendering (Apply Lighting to its range sphere, not all the screen)
    - Transparent objects rendering (Apply PR only to the mesh, not all the screen)
    - etc.
*/

in vec3 B_position_raw_vin; // Input positions

void Main()
{
    vec4 pvmPos = B_matrix_pvm * vec4(B_position_raw_vin, 1);
    gl_Position = pvmPos;
}

