#ifndef FONTSHEETCREATOR_H
#define FONTSHEETCREATOR_H

#include "Bang/G_Font.h"

/**
 * @brief The FontSheetCreator class uses the Freetype
 * library to create fontsheets. A Fontsheet is an image
 * which contains the set of glyphs of the font.
 */
FORWARD class G_Font;
FORWARD class G_Texture2D;

class G_FontSheetCreator
{
public:
    static bool LoadAtlasTexture(
                         const Path &fontFilepath,
                         int glyphSizePx,
                         G_Texture2D **resultTexture,
                         Map<char, std::pair<Vector2, Vector2> > *charAtlasUvs,
                         Map<char, G_Font::GlyphMetrics> *resultMetrics,
                         TTF_Font **fontFace);
private:
    G_FontSheetCreator();
    virtual ~G_FontSheetCreator();

    static bool Init();

    static G_FontSheetCreator *m_singleton;

};

#endif // FONTSHEETCREATOR_H
