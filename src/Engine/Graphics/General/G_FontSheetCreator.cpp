#include "Bang/G_FontSheetCreator.h"

#include <SDL2/SDL_ttf.h>

#include "Bang/Rect.h"
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
                         G_Texture2D *atlasTexture,
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

    Array<G_Image> charImages;
    for (const char c : charactersToLoadStr)
    {
        // Get some metrics
        // These are measurements relative to the full tex quad (size x size)

        int xmin, xmax, ymin, ymax, advance;
        TTF_GlyphMetrics(font, c, &xmin, &xmax, &ymin, &ymax, &advance);

        G_Font::GlyphMetrics charMetrics;
        charMetrics.size    = Vector2i((xmax - xmin), (ymax - ymin));
        charMetrics.bearing = Vector2i(xmin, ymax);
        charMetrics.advance = advance;
        if (resultMetrics) { resultMetrics->Add(c, charMetrics); }

        if (charMetrics.size.x > 0 && charMetrics.size.y > 0)
        {
            Vector2i localMinPixel(Math::Max<int>());
            Vector2i localMaxPixel(Math::Min<int>());

            // Create bitmap
            SDL_Surface *charBitmap = TTF_RenderGlyph_Blended(font, c,
                                                              WhiteColor);
            SDL_PixelFormat *fmt = charBitmap->format;
            Uint32 *charPixels = SCAST<Uint32*>(charBitmap->pixels);
            G_Image charImage(charBitmap->w, charBitmap->h);
            for(int y = 0; y < charBitmap->h; ++y)
            {
                for(int x = 0; x < charBitmap->w; ++x)
                {
                    Uint32 color32 = charPixels[y * charBitmap->w + x];
                    Uint32 alpha = ((color32 & fmt->Amask) >> fmt->Ashift)
                                    << fmt->Aloss;
                    Color pxColor = Color::Zero;
                    pxColor.a = (alpha / 255.0f);
                    if (pxColor.a > 0.0f)
                    {
                        const Vector2i xy(x,y);
                        localMinPixel = Vector2i::Min(localMinPixel, xy);
                        localMaxPixel = Vector2i::Max(localMaxPixel, xy);
                    }
                    charImage.SetPixel(x, y, pxColor);
                }
            }

            Vector2i actualCharSize = localMaxPixel - localMinPixel;

            G_Image fittedCharImage(actualCharSize.x, actualCharSize.y);
            fittedCharImage.Copy(charImage,
                                 Recti(localMinPixel, localMaxPixel),
                                 Recti(Vector2i::Zero, actualCharSize));
            // fittedCharImage.SaveToFile( Path("Char_" + String(int(c)) + ".png"));
            charImages.PushBack(fittedCharImage);
            SDL_FreeSurface(charBitmap);
        }
    }

    // Resize the atlas to fit only the used area
    Array<Recti> packedRects;
    G_Image atlasImage = G_FontSheetCreator::PackImages(charImages,
                                                        2 + extraMargin,
                                                        &packedRects);
    // atlasImage.SaveToFile(Path("font.png"));

    if (charAtlasUvs)
    {
        for (int i = 0; i < packedRects.Size(); ++i)
        {
            const Recti &packedRect = packedRects[i];
            Vector2 uvMin = Vector2(packedRect.GetMin()) /
                            Vector2(atlasImage.GetSize());
            Vector2 uvMax = Vector2(packedRect.GetMax()) /
                            Vector2(atlasImage.GetSize());
            uvMin.y       = 1.0 - uvMin.y;
            uvMax.y       = 1.0 - uvMax.y;
            charAtlasUvs->Add(charactersToLoadStr[i],
                              std::make_pair(uvMin, uvMax) );
        }
    }

    if (atlasTexture)
    {
        GL::PixelStore(GL_UNPACK_ALIGNMENT, 1);
        atlasTexture->LoadFromImage(atlasImage);
        atlasTexture->SetWrapMode(GL::WrapMode::ClampToEdge);
        atlasTexture->SetFilterMode(GL::FilterMode::Trilinear_LL);
        atlasTexture->SetAlphaCutoff(0.0f);
        atlasTexture->Bind();
        atlasTexture->GenerateMipMaps();
        atlasTexture->UnBind();
    }

    if (ttfFont) { *ttfFont = font; }

    return true;
}

G_Image G_FontSheetCreator::PackImages(const Array<G_Image> &images,
                                       int margin,
                                       Array<Recti> *imagesOutputRects)
{
    int maxImgWidth  = 0;
    int maxImgHeight = 0;
    for (const G_Image &img : images)
    {
        maxImgWidth  = Math::Max(maxImgWidth,  img.GetWidth());
        maxImgHeight = Math::Max(maxImgHeight, img.GetHeight());
    }

    int imagesPerSide = Math::Sqrt(images.Size());
    int totalWidth  = (imagesPerSide+1) * (maxImgWidth  + (margin * 2));
    int totalHeight = (imagesPerSide+1) * (maxImgHeight + (margin * 2));

    G_Image result;
    result.Create(totalWidth, totalHeight, Color::Zero);

    Vector2i penPosTopLeft(0, margin);
    for (const G_Image &img : images)
    {
        if (penPosTopLeft.x + img.GetWidth() + margin * 2 > totalWidth)
        {
            penPosTopLeft.x = 0; // Go to next line
            penPosTopLeft.y += maxImgHeight + margin * 2;
        }
        penPosTopLeft.x += margin;

        Recti imgRect = Recti(penPosTopLeft, penPosTopLeft + img.GetSize());
        imagesOutputRects->PushBack(imgRect);
        result.Copy(img, imgRect);

        penPosTopLeft.x += img.GetWidth() + margin;
    }

    Vector2i minPixel(Math::Max<int>());
    Vector2i maxPixel(Math::Min<int>());
    for (int y = 0; y < result.GetHeight(); ++y)
    {
        for (int x = 0; x < result.GetWidth(); ++x)
        {
            if ( result.GetPixel(x,y).a > 0 )
            {
                Vector2i xy(x,y);
                minPixel = Vector2i::Min(minPixel, xy);
                maxPixel = Vector2i::Max(maxPixel, xy);
            }
        }
    }

    Vector2i fittedSize = (maxPixel - minPixel);
    Vector2i fittedSizeMargined = fittedSize +Vector2i(margin * 2);
    G_Image fittedResult(fittedSizeMargined.x, fittedSizeMargined.y);
    fittedResult.Copy(result, Recti(minPixel, maxPixel),
                      Vector2i(margin) + Recti(Vector2i::Zero, fittedSize));

    return fittedResult;
}

