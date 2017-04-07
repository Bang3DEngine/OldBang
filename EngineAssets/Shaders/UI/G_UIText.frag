#include "G.frag"

void Main()
{
    float inDepth = B_SampleDepth();
    vec4 backgroundColorToBlendWith = vec4(0.0);
    if (inDepth > 0.5)
    {
        // We are on top of another char, just blend with diffuse
        // (the previous char has a full quad blended with the color buffer,
        //  so everything will be okay)
        backgroundColorToBlendWith = B_SampleDiffColor();
    }
    else
    {
        // We are not on top of another char, blend with color buffer
        backgroundColorToBlendWith = B_SampleColor();
    }

    vec4 charTexColor = texture2D(B_Texture0, B_FragIn_Uv);
    B_Out_DiffColor = mix(backgroundColorToBlendWith,
                          charTexColor * B_MaterialDiffuseColor,
                          charTexColor.a);
}
