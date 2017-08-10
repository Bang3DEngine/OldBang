#include "Bang/DistanceFielder.h"

#include "Bang/Debug.h"

void DistanceFielder::CreateDistanceField(const G_Image &inputImageBW,
                                          G_Image *outImg,
                                          int radius)
{
    outImg->Create(inputImageBW.GetWidth(), inputImageBW.GetHeight(),
                   Color(1,1,1,0));

    for (int y = 0; y < inputImageBW.GetHeight(); ++y)
    {
        const int minY = Math::Max(0, y - radius);
        const int maxY = Math::Min(inputImageBW.GetHeight()-1, y + radius);
        for (int x = 0; x < inputImageBW.GetWidth(); ++x)
        {
            const int minX = Math::Max(0, x - radius);
            const int maxX = Math::Min(inputImageBW.GetWidth()-1, x + radius);

            Color currentColor = inputImageBW.GetPixel(x, y);
            if (currentColor.a > 0)
            {
                outImg->SetPixel(x, y, Color::Black);
                continue;
            }

            Vector2 currentXY(x, y);
            float minDist = Math::Infinity<float>();
            for (int ry = minY; ry <= maxY; ++ry)
            {
                for (int rx = minX; rx <= maxX; ++rx)
                {
                    Color rColor = inputImageBW.GetPixel(rx, ry);
                    if (rColor.a > 0)
                    {
                        float dist  = Vector2::Distance(currentXY,
                                                        Vector2(rx,ry));
                        minDist = Math::Min(minDist, dist);
                    }
                }
            }
            minDist /= radius;
            if (minDist <= 1.0f)
            {
                outImg->SetPixel(x, y, Color(minDist, minDist, minDist, 1.0f));
            }
        }
    }
}
