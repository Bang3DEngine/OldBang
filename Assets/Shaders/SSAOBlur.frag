#include "PP.frag"

uniform int B_BlurRadius = 1;

void main()
{
    const float DepthEpsilon = 0.01;

    int totalPixels = 0;
    float meanOcclusion = 0.0;
    float depth = B_SampleDepth();
    for (int i = -B_BlurRadius; i <= B_BlurRadius; ++i)
    {
        for (int j = -B_BlurRadius; j <= B_BlurRadius; ++j)
        {
            float ijDepth = B_SampleDepthOffset( vec2(i,j) );
            if ( abs(depth - ijDepth) < DepthEpsilon )
            {
                meanOcclusion += B_SampleColorOffset( vec2(i,j) ).r;
                totalPixels += 1;
            }
        }
    }

    meanOcclusion /= max(totalPixels, 1);
    B_GIn_Color = vec4( vec3(meanOcclusion), 1 );
}
