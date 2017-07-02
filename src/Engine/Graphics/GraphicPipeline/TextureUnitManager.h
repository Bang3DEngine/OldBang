#ifndef TEXTUREUNITMANAGER_H
#define TEXTUREUNITMANAGER_H

#include <queue>

#include "Bang/GL.h"
#include "Bang/Map.h"

class Texture;
class TextureUnitManager
{
public:
    typedef GLuint TexUnit;
    TextureUnitManager();

    // Returns the texture unit it has been bound to
    static TexUnit BindTexture(const Texture *tex);
    static void UnBindTexture(const Texture *tex);

private:
    typedef Map<const Texture*, TexUnit> TexUnitMap;
    TexUnitMap m_textureToUnit;

    // Ordered in time
    std::queue<TexUnit> m_usedUnits;

    int c_numTextureUnits = 0;
};

#endif // TEXTUREUNITMANAGER_H
