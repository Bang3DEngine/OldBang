#ifndef FONT_H
#define FONT_H

#include "ft2build.h"
#include "freetype.h"

#include "Array.h"
#include "Asset.h"

class Texture2D;
class Font : public Asset
{
public:
    /**
     * @brief Structure to hold metrics for a character glyph.
     * They are all relative to the baseline.
     */
    struct CharGlyphMetrics
    {
        int width, height; // Width and height of character (of the actual char pixels)
        int bearingY; // Offset upwards from the baseline where the char pixels begin
        int bearingX; // Offset to right from the pen position where the char pixels begin
        int advance;  // Distance to be moved in X to right when drawing the next character
        int originY;
    };

    static String GetFileExtensionStatic();
    virtual String GetFileExtension();

    static int CharLoadSize;


    Font();
    virtual ~Font();

    void LoadFromFile(const String &m_assetFilepath);

    virtual void ReadXMLInfo(const XMLNode *xmlInfo) override;
    virtual void FillXMLInfo(XMLNode *xmlInfo) const override;

    const Font::CharGlyphMetrics &GetCharacterMetrics(unsigned char c);
    Texture2D *GetCharacterTexture(unsigned char c) const;

    /**
     * @brief Returns the distance that the pen position must be moved between
     * leftChar and rightChar (kerning)
     */
    int GetKerningX(char leftChar, char rightChar);

private:

    FT_Face m_freetypeFace;

    /**
     * @brief Each position i contains the character
     * glyph metrics for the character i. This is,
     * the bearingX, bearingY, height, width, etc.
     */
    Array<CharGlyphMetrics> m_charMetrics;

    /**
     * @brief Each position i contains the texture
     * corresponding for the character i.
     */
    Array<Texture2D*> m_charTextures;

    void Free();
};

#endif // FONT_H
