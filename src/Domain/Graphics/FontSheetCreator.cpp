#include "FontSheetCreator.h"

#include "Debug.h"
#include "Texture2D.h"

#include "ftglyph.h"

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

int FontSheetCreator::GetGlyphIndex(FT_Face face, char c)
{
    int glyph_index = FT_Get_Char_Index(face, c);
    return glyph_index;
}

bool FontSheetCreator::LoadCharTexture(const String &fontFilepath,
                                       int glyphSizePx, char character,
                                       Texture2D **resultTexture,
                                       Font::CharGlyphMetrics *resultMetrics,
                                       FT_Face *fontFace)
{
    if (!FontSheetCreator::Init()) { return false; }

    // Load the face
    FT_Face face;
    int error = FT_New_Face(m_singleton->m_ftLibrary, fontFilepath.c_str(), 0, &face);
    if (error)
    {
        Debug_Error("Failed to load font '" << fontFilepath << "': Error(" << error << ")");
        return false;
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
        return false;
    }

    error = FT_Load_Glyph(face, FontSheetCreator::GetGlyphIndex(face, character), FT_LOAD_DEFAULT);
    if (error)
    {
        Debug_Error("Failed to load glyph '" << character << "': Error(" << error << ")");
        return false;
    }

    // Move the face's glyph into a glyph object.
    FT_Glyph glyph;
    error = FT_Get_Glyph( face->glyph, &glyph );
    if (error)
    {
        Debug_Error("Failed to get glyph '" << character << "' for font '" << fontFilepath << "': Error(" << error << ")");
        return false;
    }


    // Convert the glyph to a bitmap.
    FT_Vector  origin; origin.x = origin.y = 0;
    FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, &origin, true);
    FT_BitmapGlyph bitmap_glyph = FT_BitmapGlyph(glyph);
    FT_Bitmap &bitmap = bitmap_glyph->bitmap;


    // Get some metrics (they are given in 1/64 pixels...)
    // These are measurements relative to the full texture quad (size x size)
    Font::CharGlyphMetrics metrics;
    metrics.width    =  face->glyph->metrics.width  / 64;
    metrics.height   =  face->glyph->metrics.height / 64;
    metrics.bearingX = (face->glyph->metrics.horiBearingX) / 64;
    metrics.bearingY = (face->glyph->metrics.horiBearingY) / 64;
    metrics.advance  = (face->glyph->metrics.horiAdvance)  / 64;
    metrics.originY  =  origin.y / 64;

    Texture2D *fontTexture = new Texture2D();
    if (metrics.width * metrics.height > 0)
    {
        // Create a RGBA bitmap from the pixmap provided by FreeType
        unsigned char* colorMap = new unsigned char[metrics.width * metrics.height * 4];
        for(int y = 0; y < metrics.height; y++)
        {
            for(int x = 0; x < metrics.width; x++)
            {
                int k = y * metrics.width + x;
                colorMap[k * 4 + 0] = colorMap[k * 4 + 1] = colorMap[k * 4 + 2] = 255;
                colorMap[k * 4 + 3] = bitmap.buffer[k]; // Alpha is the gray
            }
        }

        // Create texture
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        fontTexture->SetWrapMode(Texture::WrapMode::ClampToEdge);
        fontTexture->SetFilterMode(Texture::FilterMode::Linear);
        fontTexture->SetGLFormat(GL_RGBA);
        fontTexture->SetGLInternalFormat(GL_RGBA);
        fontTexture->Fill(colorMap, metrics.width, metrics.height);
        delete colorMap;
    }
    else
    {
        fontTexture->CreateEmpty(16, 16);
    }

    delete bitmap.buffer;

    *resultTexture = fontTexture;
    *resultMetrics = metrics;
    *fontFace = face;
    return true;
}
