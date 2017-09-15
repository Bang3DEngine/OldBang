#include "Bang/AspectRatioMode.h"

USING_NAMESPACE_BANG

Vector2i AspectRatio::GetAspectRatioedSize(const Vector2i &targetSize,
                                           const Vector2i &currentSize,
                                           AspectRatioMode aspectRatioMode)
{
    Vector2i finalSize = targetSize;
    if (aspectRatioMode != AspectRatioMode::Ignore)
    {
        Vector2 aspectRatio(SCAST<float>(targetSize.x) / currentSize.x,
                            SCAST<float>(targetSize.y) / currentSize.y);

        bool keepExc = (aspectRatioMode == AspectRatioMode::KeepExceeding);
        float ar =  (aspectRatio.x < aspectRatio.y) ?
                    (keepExc ? aspectRatio.y : aspectRatio.x) :
                    (keepExc ? aspectRatio.x : aspectRatio.y);
        finalSize.x = Math::Round(currentSize.x * ar);
        finalSize.y = Math::Round(currentSize.y * ar);
    }
    return finalSize;
}
