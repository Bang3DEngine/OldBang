#include "Bang/Font.h"

#include <SDL2/SDL_ttf.h>

#include "Bang/Path.h"
#include "Bang/Rect.h"
#include "Bang/Debug.h"
#include "Bang/Vector2.h"
#include "Bang/Texture2D.h"
#include "Bang/Resources.h"
#include "Bang/XMLNodeReader.h"
#include "Bang/StreamOperators.h"
#include "Bang/FontSheetCreator.h"

USING_NAMESPACE_BANG

Font::Font()
{
}

Font::~Font()
{
    Free();
}

void Font::Import(const Path &ttfFilepath)
{
    Free();

    m_ttfFilepath = ttfFilepath;

    ClearTTFError();
    m_referenceFont = TTF_OpenFont(m_ttfFilepath.GetAbsolute().ToCString(), 128);
    if (CatchTTFError() || !GetReferenceFont())
    {
        Debug_Error("Could not load font '" << ttfFilepath << "'");
        return;
    }
}

void Font::ImportXML(const XMLNode &xmlInfo)
{
    Asset::ImportXML(xmlInfo);
}

void Font::ExportXML(XMLNode *xmlInfo) const
{
    Asset::ExportXML(xmlInfo);
}

float Font::GetScaleProportion(int fontSize)
{
    return fontSize / 128.0f;
}

void Font::ClearTTFError()
{
    SDL_ClearError();
}

bool Font::CatchTTFError()
{
    const char *err = TTF_GetError();
    if (strcmp(err, "") != 0)
    {
        Debug_Error("TTF Error: " << err);
        return true;
    }
    ClearTTFError();
    return false;
}

float Font::ScaleMagnitude(int fontSize, float magnitude)
{
    return Math::Round(magnitude * GetScaleProportion(fontSize));
}

Vector2 Font::ScaleMagnitude(int fontSize, const Vector2 &magnitude)
{
    return Vector2::Round(magnitude * GetScaleProportion(fontSize));
}

Texture2D *Font::GetFontAtlas(int fontSize) const
{
    if (!HasFontSizeLoaded(fontSize))
    {
        // Create atlas
        Array<Recti> charRects;
        RH<Texture2D> atlasTex = Resources::Create<Texture2D>();
        String chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
                       "0123456789.,-;:_?!+*/\\()|\"'";

        FontSheetCreator::LoadAtlasTexture(GetTTFFont(fontSize),
                                           atlasTex.Get(), chars,
                                           &charRects, 1);

        m_cachedAtlas[fontSize] = atlasTex;
        m_cachedAtlasChars[fontSize] = chars;
        for (uint i = 0; i < chars.Size() && i < charRects.Size(); ++i)
        {
            m_cachedAtlasCharRects[fontSize].Add(chars[i], charRects[i]);
        }

        atlasTex.Get()->SetFilterMode(GL::FilterMode::Nearest);
    }

    return m_cachedAtlas.Get(fontSize).Get();
}

Font::GlyphMetrics Font::GetCharMetrics(int fontSize, char c) const
{
    Font::GlyphMetrics cm;
    if (!GetReferenceFont()) { return cm; }

    ClearTTFError();
    if (!TTF_GlyphIsProvided(GetReferenceFont(), c)) { return cm; }
    if (CatchTTFError()) { return cm; }

    ClearTTFError();
    int minx, maxx, miny, maxy, advance;
    TTF_GlyphMetrics(GetReferenceFont(), c, &minx, &maxx, &miny, &maxy, &advance);
    if (CatchTTFError()) { return cm; }

    cm.size    = ScaleMagnitude(fontSize, Vector2((maxx - minx),
                                                  (maxy - miny)) );
    cm.bearing = ScaleMagnitude(fontSize, Vector2(minx, maxy) );
    cm.advance = ScaleMagnitude(fontSize, float(advance) );
    if (c == ' ')
    {
        cm.size = ScaleMagnitude(fontSize, Vector2(cm.advance,
                                                   GetLineSkip(fontSize)) );
    }

    return cm;
}

Vector2 Font::GetCharMaxUv(int fontSize, char c) const
{
    ASSERT( HasFontSizeLoaded(fontSize) );
    if (m_cachedAtlasCharRects.Get(fontSize).ContainsKey(c))
    {
        return Vector2(m_cachedAtlasCharRects.Get(fontSize).Get(c).GetMax()) /
               Vector2(m_cachedAtlas.Get(fontSize).Get()->GetSize());
    }
    else { return Vector2::Zero; }
}
Vector2 Font::GetCharMinUv(int fontSize, char c) const
{
    ASSERT( HasFontSizeLoaded(fontSize) );
    if (m_cachedAtlasCharRects.Get(fontSize).ContainsKey(c))
    {
        return Vector2(m_cachedAtlasCharRects.Get(fontSize).Get(c).GetMin()) /
               Vector2(m_cachedAtlas.Get(fontSize).Get()->GetSize());
    }
    else { return Vector2::Zero; }
}

bool Font::HasCharacter(char c) const
{
    ClearTTFError();
    return GetReferenceFont() && TTF_GlyphIsProvided(GetReferenceFont(), c) &&
           !CatchTTFError();
}

float Font::GetKerning(int fontSize, char leftChar, char rightChar) const
{
    ClearTTFError();
    if (!GetReferenceFont() || !TTF_GetFontKerning(GetReferenceFont()) ||
         CatchTTFError())
    {
        return -1;
    }

    #if (SDL_TTF_MAJOR_VERSION >= 2 && \
         SDL_TTF_MINOR_VERSION >= 0 && \
         SDL_TTF_PATCHLEVEL >= 14)
    {
        ClearTTFError();
        int kerning = TTF_GetFontKerningSizeGlyphs(GetReferenceFont(),
                                                   leftChar, rightChar);
        if (CatchTTFError()) { return -1; }

        return ScaleMagnitude(fontSize, kerning);
    }
    #else
    return -1;
    #endif
}

float Font::GetLineSkip(int fontSize) const
{
    if (!GetReferenceFont()) { return 0.0f; }

    ClearTTFError();
    int lineSkip = TTF_FontLineSkip(GetReferenceFont());
    if (CatchTTFError()) { return -1; }

    return ScaleMagnitude(fontSize, float(lineSkip) );
}

float Font::GetFontAscent(int fontSize) const
{
    if (!GetReferenceFont()) { return 0.0f; }

    ClearTTFError();
    int fontAscent = TTF_FontAscent(GetReferenceFont());
    if (CatchTTFError()) { return -1; }

    return ScaleMagnitude(fontSize, float(fontAscent) );
}

float Font::GetFontDescent(int fontSize) const
{
    if (!GetReferenceFont()) { return 0.0f; }

    ClearTTFError();
    int fontDescent = TTF_FontDescent(GetReferenceFont());
    if (CatchTTFError()) { return -1; }

    return ScaleMagnitude(fontSize, float(fontDescent) );
}

float Font::GetFontHeight(int fontSize) const
{
    if (!GetReferenceFont()) { return 0.0f; }

    ClearTTFError();
    int fontHeight = TTF_FontHeight(GetReferenceFont());
    if (CatchTTFError()) { return -1; }

    return ScaleMagnitude(fontSize,  float(fontHeight) );
}

Vector2i Font::GetAtlasCharRectSize(int fontSize, char c) const
{
    if (!GetReferenceFont()) { return Vector2i::Zero; }
    GetFontAtlas(fontSize); // Load if not loaded yet
    return m_cachedAtlasCharRects[fontSize][c].GetSize();
}

bool Font::HasFontSizeLoaded(int fontSize) const
{
    return GetReferenceFont() && m_cachedAtlas.ContainsKey(fontSize);
}

TTF_Font *Font::GetReferenceFont() const
{
    return m_referenceFont;
}

TTF_Font *Font::GetTTFFont(int fontSize) const
{
    if (!m_openFonts.ContainsKey(fontSize))
    {
        ClearTTFError();
        TTF_Font *font = TTF_OpenFont(m_ttfFilepath.GetAbsolute().ToCString(),
                                      fontSize);
        CatchTTFError();

        m_openFonts[fontSize] = font;
    }

    return m_openFonts.Get(fontSize);
}

void Font::Free()
{
    for (const auto& it : m_openFonts)
    {
        ClearTTFError();
        TTF_CloseFont( it.second );
        CatchTTFError();
    }
    m_openFonts.Clear();
}
