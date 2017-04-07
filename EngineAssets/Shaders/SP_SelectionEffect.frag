#include "SP.frag"

void Main()
{
    bool isOutside = (B_SampleDepth() <= 0.5);
    if (isOutside)
    {
        const vec4  outlineColor = vec4(1, 0.5, 0, 1);
        const float fillOpacity  = 0.0f;
        const int   stroke       = 2;

        float closestDepth = 0.0f;
        bool isOutline     = false;
        for (int i = -stroke; i <= stroke && !isOutline; ++i)
        {
            for (int j = -stroke; j <= stroke && !isOutline; ++j)
            {
                isOutline = B_SampleDepthOffset( vec2(i,j) ) > 0.5f;
                if (isOutline)
                {
                    closestDepth = B_SampleDepthOffset( vec2(i,j) );
                }
            }
        }

        if (isOutline)
        {
            float pixelDepth   = B_SampleDepth();
            float depthFade  = (pixelDepth < closestDepth) ? 0.4f : 1.0f;
            B_Out_Color = vec4(outlineColor.rgb, outlineColor.a * depthFade);
        }
    }
}
