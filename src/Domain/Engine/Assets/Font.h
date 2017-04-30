#ifndef FONT_H
#define FONT_H

#include "freetype/ft2build.h"
#include "freetype/freetype.h"

#include "Bang/Array.h"
#include "Bang/Asset.h"

class Texture2D;
class Font : public Asset
{
	OBJECT(Font)
    ASSET_ICON(Font, "Icons/LetterIcon.png")

public:
    constexpr static int c_charLoadSize = 256;

    /**
     * @brief Structure to hold metrics for a character glyph.
     * They are all relative to the baseline.
     */
    struct CharGlyphMetrics
    {
        int width    = 0; // Width of character (of the actual char pixels)
        int height   = 0; // Height of character (of the actual char pixels)
        int bearingY = 0; // Offset upwards from the baseline where the char pixels begin
        int bearingX = 0; // Offset to right from the pen position where the char pixels begin
        int advance  = 0; // Distance to be moved in X to right when drawing the next character
        int originY  = 0;
    };

    static String GetFileExtensionStatic();
    virtual String GetFileExtension() const override ;

    Font();
    virtual ~Font();

    void LoadFromTTF(const String &m_assetFilepath);

    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

    Font::CharGlyphMetrics GetCharacterMetrics(unsigned char c);
    Vector2 GetCharMinUvInAtlas(char c) const;
    Vector2 GetCharMaxUvInAtlas(char c) const;
    Texture2D *GetAtlasTexture() const;

    /**
     * @brief Returns the distance that the pen position must be moved between
     * leftChar and rightChar (kerning)
     */
    int GetKerningX(char leftChar, char rightChar);

private:

    Path m_fontFilepath;
    FT_Face m_freetypeFace = nullptr;

    // A big texture with all the chars in it
    Texture2D *m_atlasTexture = nullptr;

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

#endif // FONT_H
