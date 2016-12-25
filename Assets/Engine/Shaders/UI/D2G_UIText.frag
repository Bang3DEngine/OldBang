#include "D2G.frag"

void Main()
{
    // Stencil has to 1 only the pixels that have been drawn by text before
    // This is useful to render overlapping character quads because:
    //   - You cant simply cut-off discard, if u want to antialias with alpha
    //   - You cant simply blend with diffuse buffer, since you could have below
    //     some stuff with PR.
    //   - You cant simply blend with color, because you are rendering all the string
    //     of quad chars in one D2G pass. So you are not writing to the color buffer in
    //     any moment.
    // The solution is to use the stencil to know when we are on top of a pixel previously written
    // by another char or not:
    //   - If it is, then we blend with the diffuse (to blend with the other char)
    //   - If its not, then we blend with the color buffer (total transparency)

    if (B_hasTexture)
    {
        vec4 backgroundColorToBlendWith = vec4(0.0);
        if (B_vin.stencil > 0.5)
        {
            // We are on top of another char, just blend with diffuse
            // ( the previous char has a full quad blended with the color buffer,
            //   so everything will be okay)
            backgroundColorToBlendWith = B_vin.diffuseColor;
        }
        else
        {
            // We are not on top of another char, blend with color buffer
            backgroundColorToBlendWith = B_vin.color;
        }

        vec4 charTexColor = texture2D(B_texture_0, B_vin.uv);
        B_vout.diffuseColor = mix(backgroundColorToBlendWith,
                                  charTexColor * B_material_diffuse_color,
                                  charTexColor.a);
    }
}
