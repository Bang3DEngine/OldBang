#include "Bang/FontSheetCreator.h"

#include <SDL2/SDL_ttf.h>

#include "Bang/GL.h"
#include "Bang/Math.h"
#include "Bang/Array.h"
#include "Bang/Debug.h"
#include "Bang/AARect.h"
#include "Bang/Vector2.h"
#include "Bang/Resources.h"
#include "Bang/Texture2D.h"
#include "Bang/ImageEffects.h"

USING_NAMESPACE_BANG

bool FontSheetCreator::LoadAtlasTexture(TTF_Font *ttfFont,
                                        Texture2D *atlasTexture,
                                        const String &charsToLoad,
                                        Array<AARecti> *imagesOutputRects,
                                        int extraMargin)
{
    if (!ttfFont) { return false; }

    Array<Imageb> charImages;
    for (const char c : charsToLoad)
    {
        if (TTF_GlyphIsProvided(ttfFont, c))
        {
            // Create bitmap
            constexpr SDL_Color WhiteColor = {255, 255, 255, 255};
            TTF_SetFontHinting(ttfFont, TTF_HINTING_LIGHT);
            SDL_Surface *charBitmap = TTF_RenderGlyph_Blended(ttfFont, c,
                                                              WhiteColor);
            if (!charBitmap) { continue; }

            SDL_PixelFormat *fmt = charBitmap->format;
            Uint32 *charPixels = SCAST<Uint32*>(charBitmap->pixels);
            Imageb charImage(charBitmap->w, charBitmap->h);
            for(int y = 0; y < charBitmap->h; ++y)
            {
                for(int x = 0; x < charBitmap->w; ++x)
                {
                    Uint32 color32 = charPixels[y * charBitmap->w + x];
                    Uint32 alpha = ((color32 & fmt->Amask) >> fmt->Ashift)
                                    << fmt->Aloss;
                    Color pxColor = Color::Zero;
                    pxColor.a = (alpha / 255.0f);
                    charImage.SetPixel(x, y, pxColor);
                }
            }
            // charImage.Export( Path("Char_" + String(int(c)) + "_" + String(charBitmap->w) + ".png") );

            charImages.PushBack(charImage);
            SDL_FreeSurface(charBitmap);
        }
        else
        {
            Imageb empty; empty.Create(1, 1, Color::Zero);
            charImages.PushBack(empty);
        }
    }

    // Resize the atlas to fit only the used area
    Imageb atlasImage = FontSheetCreator::PackImages(charImages,
                                                     extraMargin,
                                                     imagesOutputRects);
    // atlasImage.Export(Path("font_" + String(atlasImage.GetWidth()) + ".png"));

    if (atlasTexture) // Create final atlas texture
    {
        GL::PixelStore(GL::UnPackAlignment, 1);
        atlasTexture->Import(atlasImage);
        atlasTexture->SetWrapMode(GL::WrapMode::ClampToEdge);
        atlasTexture->SetFilterMode(GL::FilterMode::Nearest);
        atlasTexture->SetAlphaCutoff(0.5f);
        atlasTexture->Bind();
        atlasTexture->GenerateMipMaps();
        atlasTexture->UnBind();
    }

    return true;
}

Imageb FontSheetCreator::PackImages(const Array<Imageb> &images,
                                    int margin,
                                    Array<AARecti> *imagesOutputRects,
                                    const Color &bgColor)
{
    int maxImgWidth  = 0;
    int maxImgHeight = 0;
    for (const Imageb &img : images)
    {
        maxImgWidth  = Math::Max(maxImgWidth,  img.GetWidth());
        maxImgHeight = Math::Max(maxImgHeight, img.GetHeight());
    }

    int imagesPerSide = Math::Sqrt(images.Size())+1;
    int totalWidth  = (imagesPerSide) * (maxImgWidth  + (margin * 2));
    int totalHeight = (imagesPerSide) * (maxImgHeight + (margin * 2));

    Imageb result;
    result.Create(totalWidth, totalHeight, Color::Zero);

    maxImgHeight = 0;
    int currentRowImages = 0;
    Vector2i penPosTopLeft(0, margin);
    for (const Imageb &img : images)
    {
        if (penPosTopLeft.x + img.GetWidth() + margin * 2 > totalWidth ||
            currentRowImages >= imagesPerSide)
        {
            penPosTopLeft.x = 0; // Go to next line
            penPosTopLeft.y += maxImgHeight + margin * 2;
            currentRowImages = 0;
            maxImgHeight = 0;
        }
        ++currentRowImages;
        penPosTopLeft.x += margin;

        AARecti imgRect = AARecti(penPosTopLeft, penPosTopLeft + img.GetSize());
        if (imagesOutputRects) { imagesOutputRects->PushBack(imgRect); }
        result.Copy(img, imgRect);

        maxImgHeight = Math::Max(maxImgHeight, imgRect.GetSize().y);
        penPosTopLeft.x += imgRect.GetWidth() + margin;
    }

    Vector2i minPixel = result.GetSize();
    Vector2i maxPixel = Vector2i::Zero;
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

    maxPixel += Vector2i::One;
    Vector2i fittedSize = (maxPixel - minPixel);
    Imageb fittedResult(fittedSize.x, fittedSize.y);
    fittedResult = result.GetSubImage( AARecti(minPixel, maxPixel) );
    fittedResult.FillTransparentPixels(bgColor);

    Imageb fittedResultMargined = fittedResult;
    fittedResultMargined.AddMargins(Vector2i(margin), bgColor,
                                    AspectRatioMode::Ignore);
    return fittedResultMargined;
}
