#include "PP.frag"

int BlurSize = 5;

void main()
{
    vec4 meanColor = vec4(0);
    for (int i = -BlurSize; i <= BlurSize; ++i)
    {
        for (int j = -BlurSize; j <= BlurSize; ++j)
        {
            meanColor += B_SampleColorOffset( vec2(i,j) );
        }
    }

    meanColor /= pow(BlurSize * 2 + 1, 2);
    B_GIn_Color = meanColor;
}
