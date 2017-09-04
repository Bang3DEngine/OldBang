#ifndef G_FONT_H
#define G_FONT_H

#include <SDL2/SDL_ttf.h>

#include "Bang/Map.h"
#include "Bang/Vector2.h"
#include "Bang/Resource.h"

FORWARD class G_Texture2D;

class G_Font
{
public:
    /**
     * @brief Structure to hold metrics for a character glyph.
     * They are all relative to the baseline.
     */
    struct GlyphMetrics
    {
        Vector2i size = Vector2i::Zero; // Size of character (of the actual char pixels)
        Vector2i bearing = Vector2i::Zero; // Offset from the baseline where the char pixels begin
        int advance  = 0; // Distance to be moved in X to right when drawing the next character
    };

    G_Font();
    virtual ~G_Font();

    void SetLoadSize(int loadSize);

    int GetLoadSize() const;
    bool IsUsingDistanceField() const;
    G_Font::GlyphMetrics GetCharMetrics(unsigned char c, int fontSize = 1) const;
    Vector2 GetCharMinUvInAtlas(char c) const;
    Vector2 GetCharMaxUvInAtlas(char c) const;
    bool HasCharacter(char c) const;
    G_Texture2D *GetAtlasTexture() const;
    int GetKerningXPx(char leftChar, char rightChar) const;
    int GetLineSkipPx() const;

    TTF_Font *GetTTFFont() const;

    template<class T>
    static T ScaleMagnitude(const T &magnitude, int fontSize)
    {
        const float proportion = float(fontSize) / 128.0f;
        return SCAST<T>(magnitude * proportion);
    }

    void Import(const Path &ttfFilepath);

protected:
    int m_ttfLoadSize = 128;
    TTF_Font *m_ttfFont = nullptr;
    bool m_usingDistanceField = false;
    G_Texture2D *m_atlasTexture = nullptr;

    Map<char, std::pair<Vector2, Vector2> > m_charUvsInAtlas;
    void Free();
};

#endif // G_FONT_H
