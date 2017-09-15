#ifndef ASPECTRATIOMODE
#define ASPECTRATIOMODE

#include "Bang/Bang.h"
#include "Bang/Math.h"
#include "Bang/Vector2.h"

NAMESPACE_BANG_BEGIN

enum class AspectRatioMode {Ignore, Keep, KeepExceeding};


namespace AspectRatio
{
    Vector2i GetAspectRatioedSize(const Vector2i &targetSize,
                                  const Vector2i &currentSize,
                                  AspectRatioMode aspectRatioMode);
}

NAMESPACE_BANG_END

#endif
