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

bool G_FontSheetCreator::LoadAtlasTexture(TTF_Font *ttfFont,
                                          G_Texture2D *atlasTexture,
                                          const String &charsToLoad,
                                          Array<Recti> *imagesOutputRects,
                                          int extraMargin)
{
    if (!ttfFont) { return false; }

    Array<G_Image> charImages;
    for (const char c : charsToLoad)
    {
        if (TTF_GlyphIsProvided(ttfFont, c))
        {
            Vector2i localMinPixel(Math::Max<int>());
            Vector2i localMaxPixel(Math::Min<int>());

            // Create bitmap
            constexpr SDL_Color WhiteColor = {255, 255, 255, 255};
            SDL_Surface *charBitmap = TTF_RenderGlyph_Blended(ttfFont, c,
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

            // Fit image to the actual character size
            // (eliminate all margins/paddings)
            Vector2i actualCharSize = localMaxPixel - localMinPixel;

            G_Image fittedCharImage(actualCharSize.x, actualCharSize.y);
            fittedCharImage.Copy(charImage,
                                 Recti(localMinPixel, localMaxPixel),
                                 Recti(Vector2i::Zero, actualCharSize));
            // fittedCharImage.Export( Path("Char_" + String(int(c)) + ".png"));
            charImages.PushBack(fittedCharImage);
            SDL_FreeSurface(charBitmap);
        }
    }

    // Resize the atlas to fit only the used area
    G_Image atlasImage = G_FontSheetCreator::PackImages(charImages,
                                                        2 + extraMargin,
                                                        imagesOutputRects);
    // atlasImage.Export(Path("font.png"));

    if (atlasTexture) // Create final atlas texture
    {
        GL::PixelStore(GL_UNPACK_ALIGNMENT, 1);
        atlasTexture->Import(atlasImage);
        atlasTexture->SetWrapMode(GL::WrapMode::ClampToEdge);
        atlasTexture->SetFilterMode(GL::FilterMode::Trilinear_LL);
        atlasTexture->SetAlphaCutoff(0.0f);
        atlasTexture->Bind();
        atlasTexture->GenerateMipMaps();
        atlasTexture->UnBind();
    }

    return true;
}

G_Image G_FontSheetCreator::PackImages(const Array<G_Image> &images,
                                       int margin,
                                       Array<Recti> *imagesOutputRects,
                                       const Color &bgColor)
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
    result.Create(totalWidth, totalHeight, bgColor);

    maxImgHeight = 0;
    Vector2i penPosTopLeft(0, margin);
    for (const G_Image &img : images)
    {
        if (penPosTopLeft.x + img.GetWidth() + margin * 2 > totalWidth)
        {
            penPosTopLeft.x = 0; // Go to next line
            penPosTopLeft.y += maxImgHeight + margin * 2;
            maxImgHeight = 0;
        }
        penPosTopLeft.x += margin;

        Recti imgRect = Recti(penPosTopLeft, penPosTopLeft + img.GetSize());
        if (imagesOutputRects) { imagesOutputRects->PushBack(imgRect); }
        maxImgHeight = Math::Max(maxImgHeight, imgRect.GetSize().y);
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

