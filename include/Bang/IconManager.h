#ifndef ICONMANAGER_H
#define ICONMANAGER_H

#include "Bang/ResourceHandle.h"

NAMESPACE_BANG_BEGIN

FORWARD class Texture2D;

class IconManager
{
public:
    static void GetRightArrowIcon(RH<Texture2D> *iconTexture);
    static void GetDownArrowIcon(RH<Texture2D> *iconTexture);

private:
    IconManager() = default;
    virtual ~IconManager() = default;

    static void GetIconTexture(RH<Texture2D> *iconTexture, const String &filename);
};

NAMESPACE_BANG_END

#endif // ICONMANAGER_H

