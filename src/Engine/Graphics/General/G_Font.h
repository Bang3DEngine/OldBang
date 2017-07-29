#ifndef G_FONT_H
#define G_FONT_H

#include "freetype/ft2build.h"
#include FT_FREETYPE_H

#include "Bang/Map.h"
#include "Bang/Vector2.h"

FORWARD class Path;
FORWARD class G_Texture2D;

class G_Font
{
public:
    /**
     * @brief Structure to hold metrics for a character glyph.
     * They are all relative to the baseline.
     */
    struct CharGlyphMetrics
    {
        Vector2i size = Vector2i::Zero; // Size of character (of the actual char pixels)
        Vector2i bearing = Vector2i::Zero; // Offset from the baseline where the char pixels begin
        int advance  = 0; // Distance to be moved in X to right when drawing the next character
        int originY  = 0;
    };

    constexpr static int c_charLoadSize = 256;

    G_Font();
    virtual ~G_Font();

    virtual void LoadFromTTF(const Path &m_assetFilepath);

    G_Font::CharGlyphMetrics GetCharacterMetrics(unsigned char c,
                                                 int textSize = 1) const;
    Vector2 GetCharMinUvInAtlas(char c) const;
    Vector2 GetCharMaxUvInAtlas(char c) const;
    G_Texture2D *GetAtlasTexture() const;

    /**
     * @brief Returns the distance that the pen position must be moved between
     * leftChar and rightChar (kerning)
     */
    int GetKerningX(char leftChar, char rightChar);

    template<class T>
    static T ScaleMagnitude(const T &magnitude, int textSize)
    {
        const float fTextSize = float(textSize) / 128.0f;
        return (magnitude * fTextSize);
    }

protected:
    FT_Face m_freetypeFace = nullptr;

    // A big texture with all the chars in it
    G_Texture2D *m_atlasTexture = nullptr;

    /**
     * @brief Char c => the character glyph metrics for the character c.
     * This is, the bearingX, bearingY, height, width, etc.
     */
    Map<char, CharGlyphMetrics> m_charMetrics;

    /**
     * @brief Char c => the <min,max> uvs in the atlas texture
     * corresponding to the character c.
     */
    Map<char, std::pair<Vector2, Vector2> > m_charUvsInAtlas;

    void Free();
};

#endif // G_FONT_H
