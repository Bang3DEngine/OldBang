#ifndef TEXTUREUNITMANAGER_H
#define TEXTUREUNITMANAGER_H

#include <queue>

#include "Bang/GL.h"
#include "Bang/Map.h"

NAMESPACE_BANG_BEGIN

FORWARD class Texture;

class TextureUnitManager
{
public:
    using TexUnit = GLuint;
    TextureUnitManager();

    // Returns the texture unit it has been bound to
    static TexUnit BindTexture(const Texture *tex);
    static void UnBindTexture(const Texture *tex);

private:
    using TexUnitMap = Map<const Texture*, TexUnit>;
    TexUnitMap m_textureToUnit;

    // Ordered in time
    ::std::queue<TexUnit> m_usedUnits;

    int c_numTextureUnits = 0;
};

NAMESPACE_BANG_END

#endif // TEXTUREUNITMANAGER_H
