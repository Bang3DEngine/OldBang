#include "PP.frag"

int Stroke = 1;
const float DepthThresh = 0.004f;

void main()
{
    vec4 StrokeColor = vec4( vec3(0.05), 1);

    float fragDepth = B_SampleDepth();
    float depthN = B_SampleDepthOffset( vec2(0, 1) );
    float depthS = B_SampleDepthOffset( vec2(0,-1) );
    float diffN  = abs(fragDepth - depthN);
    float diffS  = abs(fragDepth - depthS);
    if ( abs(diffN - diffS) >= DepthThresh )
    {
        B_GIn_Color = StrokeColor;
        return;
    }

    float depthW = B_SampleDepthOffset( vec2( 1,0) );
    float depthE = B_SampleDepthOffset( vec2(-1,0) );
    float diffW  = abs(fragDepth - depthW);
    float diffE  = abs(fragDepth - depthE);
    if ( abs(diffE - diffW) >= DepthThresh )
    {
        B_GIn_Color = StrokeColor;
        return;
    }

    vec3 fragNormal = B_SampleNormal();
    for (int i = -Stroke; i <= Stroke; ++i)
    {
        for (int j = -Stroke; j <= Stroke; ++j)
        {
            vec3 ijNormal = B_SampleNormalOffset( vec2(i,j) );
            if ( dot(fragNormal, ijNormal) <= 0.2 )
            {
                B_GIn_Color = StrokeColor;
                return;
            }
        }
    }

    B_GIn_Color = B_SampleColor();
}
