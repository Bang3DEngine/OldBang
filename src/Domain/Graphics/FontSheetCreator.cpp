#include "FontSheetCreator.h"

#include "Texture2D.h"
#include "Debug.h"

#include "freetype2/ftglyph.h"

FontSheetCreator *FontSheetCreator::m_singleton = nullptr;

FontSheetCreator::FontSheetCreator()
{
    FT_Error error = FT_Init_FreeType( &m_ftLibrary );
    if (error)
    {
        Debug_Error("Could not init FreeType library: Error(" << error <<  ")");
    }
}

FontSheetCreator::~FontSheetCreator()
{
}

bool FontSheetCreator::Init()
{
    if (FontSheetCreator::m_singleton) { return true; }

    FontSheetCreator::m_singleton = new FontSheetCreator();
    return true;
}

Texture2D* FontSheetCreator::CreateCharTexture(const String &fontFilepath,
                                               int glyphSizePx, char character)
{
    if (!FontSheetCreator::Init()) { return nullptr; }

    // Load the face
    FT_Face face;
    int error = FT_New_Face(m_singleton->m_ftLibrary, fontFilepath.c_str(), 0, &face);
    if (error)
    {
        Debug_Error("Failed to load font '" << fontFilepath << "': Error(" << error << ")");
        return nullptr;
    }

    /*
    // Set size to load glyphs as
    FT_Set_Pixel_Sizes(face, 0, glyphSizePx);

    // Disable byte-alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    */

    /*
    const int resolution = 300;
    FT_Set_Char_Size(face,
                     0, // 0 means same as height
                     glyphSizePx * 64,
                     resolution,
                     resolution);
    */

    // Set the pixel size ( rasterize (?) )
    error = FT_Set_Pixel_Sizes(face, glyphSizePx, glyphSizePx);
    if (error)
    {
        Debug_Error("Failed to set font pixel size of '" << fontFilepath << "': Error(" << error << ")");
        return nullptr;
    }


    // Get character's glyph index
    int glyph_index = FT_Get_Char_Index(face, character);
    error = FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);
    if (error)
    {
        Debug_Error("Failed to load glyph '" << character << "' for font '" << fontFilepath << "': Error(" << error << ")");
        return nullptr;
    }


    // Move the face's glyph into a glyph object.
    FT_Glyph glyph;
    error = FT_Get_Glyph( face->glyph, &glyph );
    if (error)
    {
        Debug_Error("Failed to get glyph '" << character << "' for font '" << fontFilepath << "': Error(" << error << ")");
        return nullptr;
    }


    // Convert the glyph to a bitmap.
    FT_Vector  origin; origin.x = origin.y = 0;
    FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, &origin, true);
    FT_BitmapGlyph bitmap_glyph = FT_BitmapGlyph(glyph);
    FT_Bitmap &bitmap = bitmap_glyph->bitmap;


    // Get some metrics (they are given in 1/64 pixels...)
    // These are measurements relative to the full texture quad (size x size)
    FT_BBox bbox;
    FT_Glyph_Get_CBox(glyph, FT_LOAD_NO_SCALE, &bbox);

    const int size   = glyphSizePx; // Total texture quad size (letter and its margins)
    const int baseline = (face->ascender) * (size / float(face->ascender - face->descender));

    const int left  = (face->glyph->metrics.horiBearingX) / 64;
    const int right = left + face->glyph->metrics.width / 64;

    const int top   = baseline - (face->glyph->metrics.horiBearingY) / 64;
    const int bot   = top + face->glyph->metrics.height / 64;

    // Create a RGBA bitmap from the pixmap provided by FreeType
    std::cout << character << ": " << left << ", " << right << ", " << top << ", " << bot << ", " << face->glyph->metrics.horiBearingY / 64 << std::endl;
    std::cout << character << ": " << baseline << ", " << face->ascender << ", " << face->descender << std::endl;
    unsigned char* colorMap = new unsigned char[size * size * 4];
    for(int y = 0; y < size; y++)
    {
        for(int x = 0; x < size; x++)
        {
            int k = y * size + x;
            colorMap[k * 4 + 0] = colorMap[k * 4 + 1] = colorMap[k * 4 + 2] = 255;

            int alpha = 0;
            if (x >= left && x < right && y >= top && y < bot)
            {
                int glyphK = (y-top) * bitmap.width + (x-left);
                alpha = bitmap.buffer[glyphK] < 128 ? 0 : 255;
            }
            colorMap[k * 4 + 3] = alpha;
        }
    }

    // Create texture
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    Texture2D *fontTexture = new Texture2D();
    fontTexture->SetFilterMode(Texture::FilterMode::Linear);
    fontTexture->SetGLFormat(GL_RGBA);
    fontTexture->SetGLInternalFormat(GL_RGBA);
    fontTexture->Fill(colorMap, size, size);

    // Free stuff
    delete bitmap.buffer;
    delete colorMap;

    return fontTexture;
}
