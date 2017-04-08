#include "G.frag"

void Main()
{
    vec2 charAtlasUv = B_FragIn_Uv;
    B_Out_DiffColor = vec4(vec2(charAtlasUv.x, charAtlasUv.y), 0, 1);
    float alpha = texture2D(B_Texture0, charAtlasUv).a;
     B_Out_DiffColor = vec4(vec3(alpha), alpha);
    return;

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
