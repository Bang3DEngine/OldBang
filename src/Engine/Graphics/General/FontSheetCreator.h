#ifndef FONTSHEETCREATOR_H
#define FONTSHEETCREATOR_H

#include "freetype/ft2build.h"
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
public:
    static int GetGlyphIndex(FT_Face face, char c);

    static bool LoadAtlasTexture(
                         const Path &fontFilepath,
                         int glyphSizePx,
                         Texture2D **resultTexture,
                         Map<char, std::pair<Vector2, Vector2> > *charAtlasUvs,
                         Map<char, Font::CharGlyphMetrics> *resultMetrics,
                         FT_Face *fontFace);
private:
    FontSheetCreator();
    ~FontSheetCreator();
    FT_Library m_ftLibrary;

    static bool Init();

    static FontSheetCreator *m_singleton;

};

#endif // FONTSHEETCREATOR_H
