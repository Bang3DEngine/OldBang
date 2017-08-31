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

    constexpr static int CharLoadSize = 256;

    G_Font();
    virtual ~G_Font();


    G_Font::GlyphMetrics GetCharacterMetrics(unsigned char c,
                                                 int textSize = 1) const;
    Vector2 GetCharMinUvInAtlas(char c) const;
    Vector2 GetCharMaxUvInAtlas(char c) const;
    G_Texture2D *GetAtlasTexture() const;

    /**
     * @brief Returns the distance that the pen position must be moved between
     * leftChar and rightChar (kerning)
     */
    int GetKerningXPx(char leftChar, char rightChar) const;
    int GetLineSkipPx() const;

    template<class T>
    static T ScaleMagnitude(const T &magnitude, int textSize)
    {
        const float fTextSize = float(textSize) / 128.0f;
        return SCAST<T>(magnitude * fTextSize);
    }

protected:
    TTF_Font *m_ttfFont = nullptr;
    G_Texture2D *m_atlasTexture = nullptr;

    Map<char, GlyphMetrics> m_charMetrics;
    Map<char, std::pair<Vector2, Vector2> > m_charUvsInAtlas;

    void LoadFromTTF(const Path &ttfFilepath);

    void Free();
};

#endif // G_FONT_H
