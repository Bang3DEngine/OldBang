#include "Bang/G_FontSheetCreator.h"

#include <SDL2/SDL_ttf.h>

#include "Bang/Math.h"
#include "Bang/Array.h"
#include "Bang/Debug.h"
#include "Bang/Vector2.h"
#include "Bang/G_Texture2D.h"
#include "Bang/ImageEffects.h"

G_FontSheetCreator *G_FontSheetCreator::m_singleton = nullptr;

G_FontSheetCreator::G_FontSheetCreator()
{
    if ( TTF_Init() )
    {
        Debug_Error("Could not init FreeType library: Error(" <<
                    TTF_GetError() <<  ")");
    }
}

G_FontSheetCreator::~G_FontSheetCreator()
{
    TTF_Quit();
}

bool G_FontSheetCreator::Init()
{
    if (G_FontSheetCreator::m_singleton) { return true; }

    G_FontSheetCreator::m_singleton = new G_FontSheetCreator();
    return true;
}

bool G_FontSheetCreator::LoadAtlasTexture(
                         const Path &fontFilepath,
                         int glyphSizePx,
                         G_Texture2D **atlasTexture,
                         Map<char, std::pair<Vector2, Vector2> > *charAtlasUvs,
                         Map<char, G_Font::CharGlyphMetrics> *resultMetrics,
                         TTF_Font **ttfFont)
{
    if (!G_FontSheetCreator::Init()) { return false; }

    charAtlasUvs->Clear();
    resultMetrics->Clear();

    *ttfFont = TTF_OpenFont(fontFilepath.GetAbsolute().ToCString(),
                            glyphSizePx);
    if (!*ttfFont)
    {
        Debug_Error("Error opening font " << fontFilepath << ". " <<
                    TTF_GetError());
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

        // Get some metrics
        // These are measurements relative to the full tex quad (size x size)
        G_Font::CharGlyphMetrics charMetrics;

        int xmin, xmax, ymin, ymax, advance;
        TTF_GlyphMetrics(*ttfFont, c, &xmin, &xmax, &ymin, &ymax, &advance);
        charMetrics.size.x    = (xmax - xmin);
        charMetrics.size.y    = (ymax - ymin);
        charMetrics.advance   = advance;
        charMetrics.bearing.x = xmin;
        charMetrics.bearing.y = ymax;
        resultMetrics->Add(c, charMetrics);

        const uint charRow = i / charsPerRowCol;
        const uint charCol = i % charsPerRowCol;

        Vector2i minPixel(Math::Max<int>());
        Vector2i maxPixel(Math::Min<int>());
        if (charMetrics.size.x > 0 && charMetrics.size.y > 0)
        {
            // Create bitmap
            SDL_Color white; white.r = white.b = white.g = white.a = 255;
            SDL_Surface *charBitmap = TTF_RenderGlyph_Solid(*ttfFont, c, white);
            Uint8 *charPixels = SCAST<Uint8*>(charBitmap->pixels);
            const int offX = charInAtlasSize * charCol + margin;
            const int offY = charInAtlasSize * charRow + margin;
            for(int y = 0; y < charBitmap->h; ++y)
            {
                for(int x = 0; x < charBitmap->w; ++x)
                {
                    Uint8 colorIdx = charPixels[y * charBitmap->w + x];
                    SDL_Color color = charBitmap->format->palette->colors[colorIdx];
                    Color pxColor = Color(1.0f, 1.0f, 1.0f, color.a / 255.0f);
                    if (color.a > 0)
                    {
                        minPixel.x = Math::Min(minPixel.x, (offX + x));
                        minPixel.y = Math::Min(minPixel.y, (offY + y));
                        maxPixel.x = Math::Max(maxPixel.x, (offX + x));
                        maxPixel.y = Math::Max(maxPixel.y, (offY + y));
                        atlasImage.SetPixel(offX + x, offY + y, pxColor);
                    }
                }
            }
            SDL_FreeSurface(charBitmap);
        }

        Vector2i securityOffset(3);
        minPixel -= securityOffset;
        maxPixel += securityOffset;

        Vector2 uvMin = Vector2(minPixel) / Vector2(atlasImage.GetSize());
        Vector2 uvMax = Vector2(maxPixel) / Vector2(atlasImage.GetSize());
        uvMin.y       = 1.0 - uvMin.y;
        uvMax.y       = 1.0 - uvMax.y;
        charAtlasUvs->Add(c, std::make_pair(uvMin, uvMax) );
    }

    atlasImage.SaveToFile( Path("font.png") );

    GL::PixelStore(GL_UNPACK_ALIGNMENT, 1);
    *atlasTexture = new G_Texture2D();
    (*atlasTexture)->LoadFromImage(atlasImage);
    (*atlasTexture)->SetWrapMode(GL::WrapMode::ClampToEdge);
    (*atlasTexture)->SetFilterMode(GL::FilterMode::Trilinear_LL);
    (*atlasTexture)->SetAlphaCutoff(0.0f);
    (*atlasTexture)->Bind();
    (*atlasTexture)->GenerateMipMaps();
    (*atlasTexture)->UnBind();

    return true;
}
