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
FORWARD class Texture2D;

class G_FontSheetCreator
{
public:
    static bool LoadAtlasTexture(TTF_Font *fontFace,
                                 Texture2D *resultTexture,
                                 const String &charsToLoad,
                                 Array<Recti> *imagesOutputRects = nullptr,
                                 int extraMargin = 0);

    static G_Image PackImages(const Array<G_Image> &images,
                              int margin,
                              Array<Recti> *imagesOutputRects = nullptr,
                              const Color &bgColor = Color::Zero);

    G_FontSheetCreator() = delete;
};

#endif // FONTSHEETCREATOR_H
