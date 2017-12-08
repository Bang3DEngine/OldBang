#include "Bang/Font.h"

#include "Bang/Path.h"
#include "Bang/Rect.h"
#include "Bang/Vector2.h"
#include "Bang/XMLParser.h"
#include "Bang/Texture2D.h"
#include "Bang/Resources.h"
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
    m_referenceFont =
                TTF_OpenFont(m_ttfFilepath.GetAbsolute().ToCString(), 128);
    if (!m_referenceFont)
    {
        Debug_Error("Could not load font '" << ttfFilepath << "'");
        return;
    }

    // Load distance field (if it exists)
    String fileName = m_ttfFilepath.GetName() + "_DistField";
    Path distFieldImgPath = m_ttfFilepath.WithNameExt(fileName, "png");
    Path distFieldInfoPath = m_ttfFilepath.WithNameExt(fileName, "info");
    if (false && distFieldImgPath.IsFile() && distFieldInfoPath.IsFile())
    {
        m_hasDistanceField = true;

        Imageb distFieldImg;
        distFieldImg.Import(distFieldImgPath);

        p_distFieldTexture = Resources::Create<Texture2D>();
        p_distFieldTexture.Get()->Import(distFieldImg);
        p_distFieldTexture.Get()->Bind();
        p_distFieldTexture.Get()->GenerateMipMaps();
        p_distFieldTexture.Get()->SetFilterMode(GL::FilterMode::Linear);
        p_distFieldTexture.Get()->SetWrapMode(GL::WrapMode::ClampToEdge);
        p_distFieldTexture.Get()->UnBind();

        String loadedChars = "";
        Array<Recti> charPxRects;
        XMLNode distFieldInfo = XMLParser::FromFile(distFieldInfoPath);
        for (int i = 0; i < 255; ++i)
        {
            const char c = Cast<char>(i);
            String attrName = "CharRect_" + String(i);
            if (distFieldInfo.Contains(attrName))
            {
                loadedChars.Append( String(c) );
                Recti charPxRect = distFieldInfo.Get<Recti>(attrName);
                charPxRects.PushBack(charPxRect);
            }

            attrName = "SpreadOffset_" + String(i);
            if (distFieldInfo.Contains(attrName))
            {
                Vector2i spreadOffsetPx = distFieldInfo.Get<Vector2i>(attrName);
                m_distFieldSpreadOffsetPx.Add(c, spreadOffsetPx);
            }
        }

        // Process character rects
        for (int i = 0; i < loadedChars.Size(); ++i)
        {
            const char c = loadedChars[i];
            Recti charPxRect = charPxRects[i];
            Vector2 uvMin = Vector2(charPxRect.GetMin()) /
                                 Vector2(p_distFieldTexture.Get()->GetSize());
            Vector2 uvMax = Vector2(charPxRect.GetMax()) /
                                 Vector2(p_distFieldTexture.Get()->GetSize());
            m_charUvsInDistanceFieldAtlas.Add(c, std::make_pair(uvMin, uvMax));
        }
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

float Font::ScaleMagnitude(int fontSize, float magnitude)
{
    return Math::Round(magnitude * GetScaleProportion(fontSize));
}

Vector2 Font::ScaleMagnitude(int fontSize, const Vector2 &magnitude)
{
    return Vector2::Round(magnitude * GetScaleProportion(fontSize));
}

void Font::CloneInto(ICloneable *clone) const
{
    Asset::CloneInto(clone);

    Font *fontClone = Cast<Font*>(clone);

    // TODO: Clone fonts one by one...
    fontClone->m_ttfFilepath = m_ttfFilepath;
    fontClone->m_hasDistanceField  = HasDistanceField();
}

Texture2D *Font::GetFontAtlas(int fontSize) const
{
    if (!HasFontSizeLoaded(fontSize))
    {
        // Create atlas
        Array<Recti> charRects;
        RH<Texture2D> atlasTex = Resources::Create<Texture2D>();
        String chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
                       "0123456789.,-;:_?!+*/\\\"'";

        FontSheetCreator::LoadAtlasTexture(GetTTFFont(fontSize),
                                           atlasTex.Get(), chars,
                                           &charRects, 1);

        m_cachedAtlas[fontSize] = atlasTex;
        m_cachedAtlasChars[fontSize] = chars;
        for (int i = 0; i < chars.Size(); ++i)
        {
            m_cachedAtlasCharRects[fontSize].Add(chars[i], charRects[i]);
        }

        atlasTex.Get()->SetFilterMode(GL::FilterMode::Nearest);
    }

    return m_cachedAtlas.Get(fontSize).Get();
}

bool Font::HasDistanceField() const { return m_hasDistanceField; }

Font::GlyphMetrics Font::GetCharMetrics(int fontSize, char c) const
{
    Font::GlyphMetrics charMetrics;

    int xmin, xmax, ymin, ymax, advance;
    TTF_GlyphMetrics(m_referenceFont, c,
                     &xmin, &xmax, &ymin, &ymax, &advance);
    charMetrics.size    = ScaleMagnitude(fontSize, Vector2((xmax - xmin), (ymax - ymin)) );
    charMetrics.bearing = ScaleMagnitude(fontSize,  Vector2(xmin, ymax) );
    charMetrics.advance = ScaleMagnitude(fontSize,  float(advance) );
    if (c == ' ')
    {
        charMetrics.size = ScaleMagnitude(fontSize,
                                          Vector2(advance,
                                                  GetLineSkip(fontSize)) );
    }

    return charMetrics;
}

Vector2 Font::GetCharMinUvInDistField(char c) const
{
    if (!m_charUvsInDistanceFieldAtlas.ContainsKey(c)) { return Vector2::Zero; }
    return m_charUvsInDistanceFieldAtlas.Get(c).first;
}

Vector2 Font::GetCharMaxUvInDistField(char c) const
{
    if (!m_charUvsInDistanceFieldAtlas.ContainsKey(c)) { return Vector2::Zero; }
    return m_charUvsInDistanceFieldAtlas.Get(c).second;
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
    return TTF_GlyphIsProvided(GetReferenceFont(), c);
}

Texture2D* Font::GetDistFieldTexture() const
{
    return p_distFieldTexture.Get();
}

float Font::GetKerning(int fontSize, char leftChar, char rightChar) const
{
    if (!TTF_GetFontKerning(GetReferenceFont()))
    { return -1; }

#if (SDL_TTF_MAJOR_VERSION >= 2 && \
         SDL_TTF_MINOR_VERSION >= 0 && \
         SDL_TTF_PATCHLEVEL >= 14)
        return ScaleMagnitude(
                  fontSize, TTF_GetFontKerningSizeGlyphs(GetReferenceFont(),
                                                         leftChar, rightChar));
    #else
    return -1;
    #endif
}

float Font::GetLineSkip(int fontSize) const
{
    return ScaleMagnitude(fontSize, float( TTF_FontLineSkip(GetReferenceFont()) ) );
}

float Font::GetFontAscent(int fontSize) const
{
    return ScaleMagnitude(fontSize, float( TTF_FontAscent(GetReferenceFont()) ) );
}

float Font::GetFontDescent(int fontSize) const
{
    return ScaleMagnitude(fontSize, float( TTF_FontDescent(GetReferenceFont()) ) );
}

float Font::GetFontHeight(int fontSize) const
{
    return ScaleMagnitude(fontSize,  float( TTF_FontHeight(GetReferenceFont()) ) );
}

Vector2i Font::GetAtlasCharRectSize(int fontSize, char c) const
{
    GetFontAtlas(fontSize); // Load if not loaded yet
    return m_cachedAtlasCharRects[fontSize][c].GetSize();
}

bool Font::HasFontSizeLoaded(int fontSize) const
{
    return m_cachedAtlas.ContainsKey(fontSize);
}

Vector2i Font::GetDistFieldSpreadOffsetPx(char c) const
{
    if (!m_distFieldSpreadOffsetPx.ContainsKey(c)) { return Vector2i::Zero; }
    return m_distFieldSpreadOffsetPx.Get(c);
}

TTF_Font *Font::GetReferenceFont() const
{
    return m_referenceFont;
}

TTF_Font *Font::GetTTFFont(int fontSize) const
{
    if (!m_openFonts.ContainsKey(fontSize))
    {
        TTF_Font *font = TTF_OpenFont(m_ttfFilepath.GetAbsolute().ToCString(),
                                      fontSize);
        m_openFonts[fontSize] = font;
    }

    return m_openFonts.Get(fontSize);
}

void Font::Free()
{
    m_charUvsInDistanceFieldAtlas.Clear();
    for (const auto& it : m_openFonts) { TTF_CloseFont( it.second ); }
}
