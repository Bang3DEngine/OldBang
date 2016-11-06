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

Texture2D* FontSheetCreator::CreateFontSheet(const String &fontFilepath,
                                             int glyphSizePx)
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


    // Get one glyph
    int glyph_index = FT_Get_Char_Index(face, 'g');
    error = FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);
    if (error)
    {
        Debug_Error("Failed to load glyph '" << 'A' << "' for font '" << fontFilepath << "': Error(" << error << ")");
        return nullptr;
    }


    // Move The Face's Glyph Into A Glyph Object.
    FT_Glyph glyph;
    error = FT_Get_Glyph( face->glyph, &glyph );
    if (error)
    {
        Debug_Error("Failed to get glyph '" << 'A' << "' for font '" << fontFilepath << "': Error(" << error << ")");
        return nullptr;
    }

    // Render glyph
    /*
    error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
    if (error)
    {
        Debug_Error("Failed when rendering glyph '" << 'A' << "' for font '" << fontFilepath << "': Error(" << error << ")");
        return nullptr;
    }
    */

    // Convert The Glyph To A Bitmap.
    FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, 0, true);
    FT_BitmapGlyph bitmap_glyph = FT_BitmapGlyph(glyph);
    FT_Bitmap &bitmap = bitmap_glyph->bitmap;

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    int width  = bitmap.width;
    int height = bitmap.rows;
    unsigned char* colorMap = new unsigned char[width * height * 4];
    for(int j = 0; j < height; j++)
    {
        for(int i = 0; i < width; i++)
        {
            int k = j * width + i;
            colorMap[k * 4 + 0] = 255;
            colorMap[k * 4 + 1] = 0;
            colorMap[k * 4 + 2] = 0;
            colorMap[k * 4 + 3] = bitmap.buffer[k] < 128 ? 0 : 255; // Alpha
        }
    }

    Texture2D *fontTexture = new Texture2D();
    fontTexture->SetFilterMode(Texture::FilterMode::Linear);
    fontTexture->SetGLFormat(GL_RGBA);
    fontTexture->SetGLInternalFormat(GL_RGBA);
    fontTexture->Fill(colorMap, bitmap.width, bitmap.rows);

    delete bitmap.buffer;
    delete colorMap;

    Debug_Log(bitmap.rows);
    Debug_Log(bitmap.width);
    Debug_Log(face->num_glyphs);

    return fontTexture;
}
