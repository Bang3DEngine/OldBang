#include "Bang/Font.h"

#include "Bang/Bang.h"
#include BANG_SDL2_TTF_INCLUDE(SDL_ttf.h)

#include "Bang/Path.h"
#include "Bang/Debug.h"
#include "Bang/AARect.h"
#include "Bang/Thread.h"
#include "Bang/Vector2.h"
#include "Bang/Texture2D.h"
#include "Bang/Resources.h"
#include "Bang/MutexLocker.h"
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

    const int RefFontSize = 128;
    m_referenceFont = TTF_OpenFont(m_ttfFilepath.GetAbsolute().ToCString(),
                                   RefFontSize);
    bool error = (CatchTTFError() || !GetReferenceFont());
    if (!error)
    {
        m_referenceFontDataCache.height   = float(TTF_FontHeight(GetReferenceFont()));
        m_referenceFontDataCache.ascent   = float(TTF_FontAscent(GetReferenceFont()));
        m_referenceFontDataCache.descent  = float(TTF_FontDescent(GetReferenceFont()));
        m_referenceFontDataCache.lineSkip = float(TTF_FontLineSkip(GetReferenceFont()));

        unsigned int c = 0;
        while (c <= 255)
        {
            int minx, maxx, miny, maxy, advance;
            TTF_GlyphMetrics(GetReferenceFont(), SCAST<unsigned char>(c),
                             &minx, &maxx, &miny, &maxy, &advance);

            GlyphMetrics cm;
            cm.size = Vector2((maxx - minx), (maxy - miny));
            cm.bearing = Vector2(minx, maxy);
            cm.advance = float(advance);

            if (c == ' ') { cm.size = Vector2(cm.advance,
                                              m_referenceFontDataCache.lineSkip); }

            m_referenceFontDataCache.charMetrics.Add(SCAST<unsigned char>(c), cm);
            ++c;
        }
    }
    else
    {
        Debug_Error("Could not load font '" << ttfFilepath << "'");
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
        Array<AARecti> charRects;
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
    if (!m_referenceFontDataCache.charMetrics.ContainsKey(c)) { return cm; }

    cm = m_referenceFontDataCache.charMetrics.Get(c);
    cm.size    = ScaleMagnitude(fontSize, cm.size);
    cm.bearing = ScaleMagnitude(fontSize, cm.bearing);
    cm.advance = ScaleMagnitude(fontSize, cm.advance);

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
    return GetReferenceFont() &&
           m_referenceFontDataCache.charMetrics.ContainsKey(c);
}

float Font::GetKerning(int fontSize, char leftChar, char rightChar) const
{
    return -1;
}

float Font::GetLineSkip(int fontSize) const
{
    if (!GetReferenceFont()) { return 0.0f; }
    return ScaleMagnitude(fontSize, m_referenceFontDataCache.lineSkip);
}

float Font::GetFontAscent(int fontSize) const
{
    if (!GetReferenceFont()) { return 0.0f; }
    return ScaleMagnitude(fontSize, m_referenceFontDataCache.ascent);
}

float Font::GetFontDescent(int fontSize) const
{
    if (!GetReferenceFont()) { return 0.0f; }
    return ScaleMagnitude(fontSize, m_referenceFontDataCache.descent);
}

float Font::GetFontHeight(int fontSize) const
{
    if (!GetReferenceFont()) { return 0.0f; }
    return ScaleMagnitude(fontSize, m_referenceFontDataCache.height);
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
