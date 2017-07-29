#ifndef TEXTUREUNITMANAGER_H
#define TEXTUREUNITMANAGER_H

#include <queue>

#include "Bang/GL.h"
#include "Bang/Map.h"

FORWARD class G_Texture;

class G_TextureUnitManager
{
public:
    typedef GLuint TexUnit;
    G_TextureUnitManager();

    // Returns the texture unit it has been bound to
    static TexUnit BindTexture(const G_Texture *tex);
    static void UnBindTexture(const G_Texture *tex);

private:
    typedef Map<const G_Texture*, TexUnit> TexUnitMap;
    TexUnitMap m_textureToUnit;

    // Ordered in time
    std::queue<TexUnit> m_usedUnits;

    int c_numTextureUnits = 0;
};

#endif // TEXTUREUNITMANAGER_H
