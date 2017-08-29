#include "Bang/G_FontSheetCreator.h"

#include "Bang/Math.h"
#include "Bang/Array.h"
#include "Bang/Debug.h"
#include "Bang/Vector2.h"
#include "Bang/G_Texture2D.h"
#include "Bang/DistanceFielder.h"

#include FT_GLYPH_H

G_FontSheetCreator *G_FontSheetCreator::m_singleton = nullptr;

G_FontSheetCreator::G_FontSheetCreator()
{
    FT_Error error = FT_Init_FreeType( &m_ftLibrary );
    if (error)
    {
        Debug_Error("Could not init FreeType library: Error(" << error <<  ")");
    }
}

G_FontSheetCreator::~G_FontSheetCreator()
{
}

bool G_FontSheetCreator::Init()
{
    if (G_FontSheetCreator::m_singleton) { return true; }

    G_FontSheetCreator::m_singleton = new G_FontSheetCreator();
    return true;
}

int G_FontSheetCreator::GetGlyphIndex(FT_Face face, char c)
{
    int glyph_index = FT_Get_Char_Index(face, c);
    return glyph_index;
}

bool G_FontSheetCreator::LoadAtlasTexture(
                         const Path &fontFilepath,
                         int glyphSizePx,
                         G_Texture2D **atlasTexture,
                         Map<char, std::pair<Vector2, Vector2> > *charAtlasUvs,
                         Map<char, G_Font::CharGlyphMetrics> *resultMetrics,
                         FT_Face *fontFace)
{
    if (!G_FontSheetCreator::Init()) { return false; }

    charAtlasUvs->Clear();
    resultMetrics->Clear();

    FT_Face face;
    int error = FT_New_Face(m_singleton->m_ftLibrary,
                            fontFilepath.GetAbsolute().ToCString(),
                            0, &face);
    if (error)
    {
        Debug_Error("Failed to load font '" << fontFilepath << "': Error("
                    << error << ")");
        return false;
    }

    // Set the pixel size ( rasterize (?) )
    error = FT_Set_Pixel_Sizes(face, glyphSizePx, glyphSizePx);
    //SDL_DisplayMode dm;
    //SDL_GetCurrentDisplayMode(0, &dm);
    //error = FT_Set_Char_Size(face, glyphSizePx, glyphSizePx,
    //                         dm.w, dm.h);
    if (error)
    {
        Debug_Error("Failed to set font pixel size of '" << fontFilepath
                    << "': Error(" << error << ")");
        return false;
    }

    String charactersToLoadStr = "";
    charactersToLoadStr += "abcdefghijklmnopqrstuvwxyz";
    charactersToLoadStr += "ABCDEFGHIJKLMNOPQRSTUVWXYZ ";
    charactersToLoadStr += "0123456789()[]{}*.,;:-_=!<>+";
    charactersToLoadStr += "/\\$%&@\"'#¿?^";

    // Generate the atlas, adding each char in a simple grid
    uint numChars        = charactersToLoadStr.Size();
    uint charsPerRowCol  = Math::Sqrt(numChars) + 1;
    uint charInAtlasSize = G_Font::CharLoadSize * 1.1f;
    uint margin          = 5;
    uint sideSize        = charsPerRowCol * charInAtlasSize + margin * 2;
    G_Image atlasImage(sideSize, sideSize);
    for (int i = 0; i < numChars; ++i)
    {
        const char c = charactersToLoadStr[i];

        error = FT_Load_Glyph(face, G_FontSheetCreator::GetGlyphIndex(face, c),
                              FT_LOAD_DEFAULT);
        if (error)
        {
            Debug_Error("Failed to load glyph '" << c <<
                        "': Error(" << error << ")");
            continue;
        }

        // Move the face's glyph into a glyph object.
        FT_Glyph glyph;
        error = FT_Get_Glyph( face->glyph, &glyph );
        if (error)
        {
            Debug_Error("Failed to get glyph '" << c << "' for font '"
                        << fontFilepath << "': Error(" << error << ")");
            continue;
        }


        // Convert the glyph to a bitmap.
        FT_Vector  origin; origin.x = origin.y = 0;
        FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, &origin, true);
        FT_BitmapGlyph bitmap_glyph = FT_BitmapGlyph(glyph);
        FT_Bitmap &bitmap = bitmap_glyph->bitmap;


        // Get some metrics (they are given in 1/64 pixels...)
        // These are measurements relative to the full tex quad (size x size)
        G_Font::CharGlyphMetrics charMetrics;
        charMetrics.size.x    =  face->glyph->metrics.width  / 64;
        charMetrics.size.y    =  face->glyph->metrics.height / 64;
        charMetrics.bearing.x = (face->glyph->metrics.horiBearingX) / 64;
        charMetrics.bearing.y = (face->glyph->metrics.horiBearingY) / 64;
        charMetrics.advance   = (face->glyph->metrics.horiAdvance)  / 64;
        charMetrics.originY   =  origin.y / 64;
        resultMetrics->Add(c, charMetrics);

        const uint charRow = i / charsPerRowCol;
        const uint charCol = i % charsPerRowCol;

        Vector2i minPixel(std::numeric_limits<int>::max());
        Vector2i maxPixel(std::numeric_limits<int>::min());
        if (charMetrics.size.x > 0 && charMetrics.size.y > 0)
        {
            const int offX = charInAtlasSize * charCol + margin;
            const int offY = charInAtlasSize * charRow + margin;
            for(int y = 0; y < charMetrics.size.y; y++)
            {
                for(int x = 0; x < charMetrics.size.x; x++)
                {
                    float pixelAlpha = bitmap.buffer[y * charMetrics.size.x + x];
                    Color pxColor = Color(1.0f, 1.0f, 1.0f,
                                          pixelAlpha / 255.0f);
                    minPixel.x = Math::Min(minPixel.x, (offX + x));
                    minPixel.y = Math::Min(minPixel.y, (offY + y));
                    maxPixel.x = Math::Max(maxPixel.x, (offX + x));
                    maxPixel.y = Math::Max(maxPixel.y, (offY + y));
                    atlasImage.SetPixel(offX + x, offY + y, pxColor);
                }
            }
        }

        Vector2 uvMin = Vector2(minPixel) / Vector2(atlasImage.GetSize());
        Vector2 uvMax = Vector2(maxPixel) / Vector2(atlasImage.GetSize());
        uvMin.y       = 1.0 - uvMin.y;
        uvMax.y       = 1.0 - uvMax.y;
        charAtlasUvs->Add(c, std::make_pair(uvMin, uvMax) );

        FT_Done_Glyph(glyph);
    }

    // atlasImage.SaveToFile( Path("original.png") );
    // G_Image distFieldImg;
    // DistanceFielder::CreateDistanceField(atlasImage, &distFieldImg, distRadius);
    // distFieldImg.SaveToFile( Path("distField.png") );

    GL::PixelStore(GL_UNPACK_ALIGNMENT, 1);
    *atlasTexture = new G_Texture2D();
    (*atlasTexture)->LoadFromImage(atlasImage);
    // (*atlasTexture)->LoadFromImage(distFieldImg);
    (*atlasTexture)->SetWrapMode(GL::WrapMode::ClampToEdge);
    (*atlasTexture)->SetFilterMode(GL::FilterMode::Trilinear_LL);
    (*atlasTexture)->SetAlphaCutoff(0.0f);
    (*atlasTexture)->Bind();
    (*atlasTexture)->GenerateMipMaps();
    (*atlasTexture)->UnBind();

    *fontFace = face;
    FT_Done_Face(face);
    return true;
}
