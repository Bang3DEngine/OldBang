#ifndef ICONMANAGER_H
#define ICONMANAGER_H

#include "Bang/Bang.h"

NAMESPACE_BANG_BEGIN

FORWARD class Texture2D;

class IconManager
{
public:
    static Texture2D *GetRightArrowIcon();
    static Texture2D *GetDownArrowIcon();

private:
    IconManager() = default;
    virtual ~IconManager() = default;

    static Texture2D *GetIconTexture(const String &filename);
};

NAMESPACE_BANG_END

#endif // ICONMANAGER_H

