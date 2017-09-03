#include "Bang/ImageEffects.h"

#include "Bang/Debug.h"

void ImageEffects::SignedDistanceField(const G_Image &inputImageBW,
                                       G_Image *outImg,
                                       int radius)
{
    outImg->Create(inputImageBW.GetWidth(), inputImageBW.GetHeight(),
                   Color::White);

    constexpr float negativeOffset = 0.25f;

    G_Image outline;
    ImageEffects::Outline(inputImageBW, &outline);
    // outline.Export( Path("outline.png") );

    for (int y = 0; y < inputImageBW.GetHeight(); ++y)
    {
        Debug_Peek(y);
        const int minY = Math::Max(0, y - radius);
        const int maxY = Math::Min(inputImageBW.GetHeight()-1, y + radius);
        for (int x = 0; x < inputImageBW.GetWidth(); ++x)
        {
            const int minX = Math::Max(0, x - radius);
            const int maxX = Math::Min(inputImageBW.GetWidth()-1, x + radius);

            float minDist = Math::Infinity<float>();
            for (int ry = minY; ry <= maxY; ++ry)
            {
                for (int rx = minX; rx <= maxX; ++rx)
                {
                    if (outline.GetPixel(rx,ry).r == 1.0f)
                    {
                        float dist  = Vector2::Distance(Vector2(x,y),
                                                        Vector2(rx,ry));
                        minDist = Math::Min(minDist, dist);
                    }
                }
            }

            if (minDist <= radius)
            {
                minDist /= radius;
                const bool isInterior = (inputImageBW.GetPixel(x,y).r != 0.0f);
                if (isInterior) { minDist = -minDist; }
                minDist += negativeOffset;
                minDist = Math::Clamp(minDist, 0.0f, 1.0f);
                outImg->SetPixel(x, y, Color(minDist, minDist, minDist, 1.0f));
            }
        }
    }
}

void ImageEffects::Outline(const G_Image &imgBW,
                           G_Image *outlineOutputImageBW)
{
    outlineOutputImageBW->Create(imgBW.GetWidth(), imgBW.GetHeight());
    for (int y = 0; y < imgBW.GetHeight(); ++y)
    {
        const int minY = Math::Max(0, y-1);
        const int maxY = Math::Min(imgBW.GetHeight()-1, y+1);
        for (int x = 0; x < imgBW.GetWidth(); ++x)
        {
            const int minX = Math::Max(0, x-1);
            const int maxX = Math::Min(imgBW.GetWidth()-1, x+1);

            bool isFrontier = false;
            const Color currentColor = imgBW.GetPixel(x,y);
            if (currentColor == Color::Zero)
            {
                for (int ry = minY; ry <= maxY; ++ry)
                {
                    for (int rx = minX; rx <= maxX; ++rx)
                    {
                        if (imgBW.GetPixel(rx,ry) != Color::Zero)
                        {
                            isFrontier = true;
                            // Debug_Log(currentColor << ", " << imgBW.GetPixel(rx,ry));
                        }
                    }
                }
            }
            outlineOutputImageBW->SetPixel(x, y, isFrontier ? Color::White :
                                                              Color::Zero);
        }
    }
}

bool ImageEffects::Diff(const G_Image &inputImage1,
                        const G_Image &inputImage2,
                        G_Image *diffOutputImage,
                        bool diffAlpha)
{
    diffOutputImage->Create(inputImage1.GetWidth(),
                            inputImage1.GetHeight(),
                            Color::White);

    bool diff = false;
    for (int y = 0; y < inputImage1.GetHeight(); ++y)
    {
        for (int x = 0; x < inputImage1.GetWidth(); ++x)
        {
            Color c1 = inputImage1.GetPixel(x, y);
            Color c2 = inputImage2.GetPixel(x, y);
            Color diffColor( Math::Abs(c1.r-c2.r), Math::Abs(c1.g-c2.g),
                             Math::Abs(c1.b-c2.b), Math::Abs(c1.a-c2.a) );
            if (!diffAlpha) { diffColor.a = 1.0f; }

            diffOutputImage->SetPixel(x, y, diffColor);

            if (diffColor != Color::Zero) { diff = true; }
        }
    }
    return diff;
}
