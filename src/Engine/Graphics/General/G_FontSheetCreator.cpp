#include "Bang/G_FontSheetCreator.h"

#include <SDL2/SDL_ttf.h>

#include "Bang/Math.h"
#include "Bang/Array.h"
#include "Bang/Debug.h"
#include "Bang/Vector2.h"
#include "Bang/Resources.h"
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
                         Map<char, G_Font::GlyphMetrics> *resultMetrics,
                         TTF_Font **ttfFont,
                         const String &charsToLoad,
                         int extraMargin)
{
    if (!G_FontSheetCreator::Init()) { return false; }

    constexpr SDL_Color WhiteColor = {255, 255, 255, 255};

    if (charAtlasUvs) { charAtlasUvs->Clear(); }
    if (resultMetrics) { resultMetrics->Clear(); }

    TTF_Font *font = TTF_OpenFont(fontFilepath.GetAbsolute().ToCString(),
                                  glyphSizePx);
    if (!font)
    {
        Debug_Error("Error opening font " << fontFilepath << ". " <<
                    TTF_GetError());
        return false;
    }

    String charactersToLoadStr = charsToLoad;
    if (charactersToLoadStr.IsEmpty())
    {
        charactersToLoadStr += "abcdefghijklmnopqrstuvwxyz";
        charactersToLoadStr += "ABCDEFGHIJKLMNOPQRSTUVWXYZ ";
        charactersToLoadStr += "0123456789()[]{}*.,;:-_=!<>+";
        charactersToLoadStr += "/\\$%&@\"'#¿?^";
    }

    // Generate the atlas, adding each char in a simple grid
    int numChars        = charactersToLoadStr.Size();
    int charsPerRowCol  = Math::Sqrt(numChars) + 1;
    int charInAtlasSize = glyphSizePx;
    int margin          = 2 + extraMargin;
    charInAtlasSize      = Math::Max(charInAtlasSize, TTF_FontLineSkip(font));
    charInAtlasSize     += margin * 2;
    int sideSize        = charsPerRowCol * charInAtlasSize;

    Debug_Peek(charInAtlasSize);
    Debug_Peek(TTF_FontLineSkip(font));
    Debug_Peek(margin);
    Debug_Peek(sideSize);

    G_Image atlasImage;
    atlasImage.Create(sideSize, sideSize, Color::Zero);
    Vector2i penPosTopLeft(margin);
    Vector2i minPixel( Math::Max<int>() );
    Vector2i maxPixel( Math::Min<int>() );
    for (int i = 0; i < numChars; ++i)
    {
        const char c = charactersToLoadStr[i];

        // Get some metrics
        // These are measurements relative to the full tex quad (size x size)
        G_Font::GlyphMetrics charMetrics;

        int xmin, xmax, ymin, ymax, advance;
        TTF_GlyphMetrics(font, c, &xmin, &xmax, &ymin, &ymax, &advance);
        charMetrics.size    = Vector2i((xmax - xmin), (ymax - ymin));
        charMetrics.bearing = Vector2i(xmin, ymax);
        charMetrics.advance = advance;
        if (resultMetrics) { resultMetrics->Add(c, charMetrics); }

        Vector2i localMinPixel( Math::Max<int>() );
        Vector2i localMaxPixel( Math::Min<int>() );
        if (charMetrics.size.x > 0 && charMetrics.size.y > 0)
        {
            if (penPosTopLeft.x + glyphSizePx + margin >= sideSize)
            {
                penPosTopLeft.x = margin;       // Go to next line
                penPosTopLeft.y += glyphSizePx + margin * 2;
            }
            // Create bitmap
            // SDL_Surface *charBitmap = TTF_RenderGlyph_Solid(font, c, WhiteColor);
            // Uint8 *charPixels = SCAST<Uint8*>(charBitmap->pixels);
            SDL_Surface *charBitmap = TTF_RenderGlyph_Blended(font, c,
                                                              WhiteColor);
            SDL_PixelFormat *fmt = charBitmap->format;
            Uint32 *charPixels = SCAST<Uint32*>(charBitmap->pixels);
            for(int y = 0; y < charBitmap->h; ++y)
            {
                for(int x = 0; x < charBitmap->w; ++x)
                {
                    // Uint8 colorIdx = charPixels[y * charBitmap->w + x];
                    // SDL_Color color = charBitmap->format->palette->colors[colorIdx];
                    Uint32 color32 = charPixels[y * charBitmap->w + x];
                    Uint32 alpha = ((color32 & fmt->Amask) >> fmt->Ashift)
                                    << fmt->Aloss;
                    Color pxColor = Color::Zero;
                    pxColor.a = (alpha / 255.0f);
                    if (pxColor.a > 0.0f)
                    {
                        const Vector2i xy = penPosTopLeft + Vector2i(x,y);
                        localMinPixel = Vector2i::Min(localMinPixel, xy);
                        localMaxPixel = Vector2i::Max(localMaxPixel, xy);
                    }
                    atlasImage.SetPixel(penPosTopLeft.x + x,
                                        penPosTopLeft.y + y,
                                        pxColor);
                }
            }
            SDL_FreeSurface(charBitmap);
        }

        penPosTopLeft.x = localMaxPixel.x + margin * 2;

        Vector2i securityOffset(3);
        localMinPixel -= securityOffset;
        localMaxPixel += securityOffset;

        Vector2 uvMin = Vector2(localMinPixel) / Vector2(atlasImage.GetSize());
        Vector2 uvMax = Vector2(localMaxPixel) / Vector2(atlasImage.GetSize());
        uvMin.y       = 1.0 - uvMin.y;
        uvMax.y       = 1.0 - uvMax.y;
        if (c == ' ') { uvMin = uvMax = Vector2::Zero; }
        if (charAtlasUvs) { charAtlasUvs->Add(c, std::make_pair(uvMin, uvMax) ); }

        minPixel = Vector2i::Min(minPixel, localMinPixel);
        maxPixel = Vector2i::Max(maxPixel, localMaxPixel);
    }

    minPixel -= margin;
    maxPixel += margin;

    // Resize the atlas to fit only the used area
    Vector2i fittedSize = maxPixel - minPixel;
    G_Image fittedAtlasImage;
    fittedAtlasImage.Create(fittedSize.x, fittedSize.y, Color::Zero);
    for(int y = 0; y < fittedSize.y; ++y)
    {
        for(int x = 0; x < fittedSize.x; ++x)
        {
            Vector2i originalXY = minPixel + Vector2i(x,y);
            fittedAtlasImage.SetPixel(x, y, atlasImage.GetPixel(originalXY.x,
                                                                originalXY.y));
        }
    }

    GL::PixelStore(GL_UNPACK_ALIGNMENT, 1);
    *atlasTexture = new G_Texture2D();
    (*atlasTexture)->LoadFromImage(fittedAtlasImage);
    (*atlasTexture)->SetWrapMode(GL::WrapMode::ClampToEdge);
    (*atlasTexture)->SetFilterMode(GL::FilterMode::Trilinear_LL);
    (*atlasTexture)->SetAlphaCutoff(0.0f);
    (*atlasTexture)->Bind();
    (*atlasTexture)->GenerateMipMaps();
    (*atlasTexture)->UnBind();

    if (ttfFont) { *ttfFont = font; }

    return true;
}

G_Image G_FontSheetCreator::PackImages(const Array<G_Image> &images,
                                       int margin,
                                       Array<Vector2i> *imagesCoordsPx)
{

}
