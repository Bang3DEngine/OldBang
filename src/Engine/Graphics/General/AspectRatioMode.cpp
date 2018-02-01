#include "Bang/AspectRatioMode.h"

USING_NAMESPACE_BANG

Vector2i AspectRatio::GetAspectRatioedSize(const Vector2i &currentSize,
                                           const Vector2i &targetSize,
                                           AspectRatioMode aspectRatioMode)
{
    Vector2i finalSize = targetSize;
    if (aspectRatioMode != AspectRatioMode::Ignore)
    {
        Vector2 aspectRatio(Cast<float>(targetSize.x) / currentSize.x,
                            Cast<float>(targetSize.y) / currentSize.y);

        bool keepExc = (aspectRatioMode == AspectRatioMode::KeepExceeding);
        float ar =  (aspectRatio.x < aspectRatio.y) ?
                    (keepExc ? aspectRatio.y : aspectRatio.x) :
                    (keepExc ? aspectRatio.x : aspectRatio.y);
        finalSize.x = Math::Round(currentSize.x * ar);
        finalSize.y = Math::Round(currentSize.y * ar);
    }
    return finalSize;
}
