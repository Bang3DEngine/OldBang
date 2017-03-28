#ifndef FONTSHEETCREATOR_H
#define FONTSHEETCREATOR_H

#include <ft2build.h>
#include FT_FREETYPE_H

#include "Bang/Font.h"
#include "Bang/String.h"

/**
 * @brief The FontSheetCreator class uses the Freetype
 * library to create fontsheets. A Fontsheet is an image
 * which contains the set of glyphs of the font.
 */
class Font;
class Texture2D;
class FontSheetCreator
{
private:
    FontSheetCreator();
    ~FontSheetCreator();
    FT_Library m_ftLibrary;

    static bool Init();

    static FontSheetCreator *m_singleton;

public:
    static int GetGlyphIndex(FT_Face face, char c);
    static bool LoadCharTexture(const String &fontFilepath,
                                int glyphSizePx, char character,
                                Texture2D **resultTexture,
                                Font::CharGlyphMetrics *resultMetrics,
                                FT_Face *fontFace);
};

#endif // FONTSHEETCREATOR_H
