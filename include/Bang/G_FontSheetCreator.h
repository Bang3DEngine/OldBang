#ifndef FONTSHEETCREATOR_H
#define FONTSHEETCREATOR_H

#include "Bang/G_Font.h"
#include "Bang/G_Image.h"

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
                         G_Texture2D *resultTexture,
                         Map<char, std::pair<Vector2, Vector2> > *charAtlasUvs = nullptr,
                         Map<char, G_Font::GlyphMetrics> *resultMetrics = nullptr,
                         TTF_Font **fontFace = nullptr,
                         const String &charsToLoad = "",
                         int extraMargin = 0);

    static G_Image PackImages(const Array<G_Image> &images,
                              int margin,
                              Array<Recti> *imagesOutputRects);
private:
    G_FontSheetCreator();
    virtual ~G_FontSheetCreator();

    static bool Init();

    static G_FontSheetCreator *m_singleton;

};

#endif // FONTSHEETCREATOR_H
