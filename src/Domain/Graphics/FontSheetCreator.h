#ifndef FONTSHEETCREATOR_H
#define FONTSHEETCREATOR_H

#include <ft2build.h>
#include FT_FREETYPE_H

#include "Bang.h"

#include "String.h"

/**
 * @brief The FontSheetCreator class uses the Freetype
 * library to create fontsheets. A Fontsheet is an image
 * which contains the set of glyphs of the font.
 */
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
    static Texture2D* CreateFontSheet(const String &fontFilepath,
                                      int glyphSizePx);
};

#endif // FONTSHEETCREATOR_H
