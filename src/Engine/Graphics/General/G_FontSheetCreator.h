#ifndef FONTSHEETCREATOR_H
#define FONTSHEETCREATOR_H

#include "freetype/ft2build.h"
#include FT_FREETYPE_H

#include "Bang/G_Font.h"
#include "Bang/String.h"

/**
 * @brief The FontSheetCreator class uses the Freetype
 * library to create fontsheets. A Fontsheet is an image
 * which contains the set of glyphs of the font.
 */
class G_Font;
class G_Texture2D;
class G_FontSheetCreator
{
public:
    static int GetGlyphIndex(FT_Face face, char c);

    static bool LoadAtlasTexture(
                         const Path &fontFilepath,
                         int glyphSizePx,
                         G_Texture2D **resultTexture,
                         Map<char, std::pair<Vector2, Vector2> > *charAtlasUvs,
                         Map<char, G_Font::CharGlyphMetrics> *resultMetrics,
                         FT_Face *fontFace);
private:
    G_FontSheetCreator();
    virtual ~G_FontSheetCreator();
    FT_Library m_ftLibrary;

    static bool Init();

    static G_FontSheetCreator *m_singleton;

};

#endif // FONTSHEETCREATOR_H
