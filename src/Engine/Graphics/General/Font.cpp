#include "Bang/Font.h"

#include "Bang/Path.h"
#include "Bang/Rect.h"
#include "Bang/Vector2.h"
#include "Bang/XMLParser.h"
#include "Bang/Texture2D.h"
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

    m_ttfFont = TTF_OpenFont(ttfFilepath.GetAbsolute().ToCString(),
                             GetLoadSize());
    if (!GetTTFFont())
    {
        Debug_Error("Error opening font " << ttfFilepath << ". " <<
                    TTF_GetError());
        return;
    }

    String fileName = ttfFilepath.GetName() + "_DistField";
    Path distFieldImgPath = ttfFilepath.WithNameExt(fileName, "png");
    Path distFieldInfoPath = ttfFilepath.WithNameExt(fileName, "info");

    String loadedChars = "";
    Array<Recti> charPxRects;
    m_atlasTexture = Asset::Create<Texture2D>();
    if (distFieldImgPath.IsFile() && distFieldInfoPath.IsFile())
    {
        Imageb distFieldImg;
        distFieldImg.Import(distFieldImgPath);
        m_atlasTexture->Import(distFieldImg);

        XMLNode distFieldInfo = XMLParser::FromFile(distFieldInfoPath);

        if (distFieldInfo.Contains("LoadSize"))
        { SetLoadSize( distFieldInfo.Get<int>("LoadSize") ); }

        for (int i = 0; i < 255; ++i)
        {
            const char c = SCAST<char>(i);
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
                m_sdfSpreadOffsetPxInAtlas.Add(c, spreadOffsetPx);
            }
        }
        m_atlasTexture->Bind();
        m_atlasTexture->GenerateMipMaps();
        m_atlasTexture->SetFilterMode(GL::FilterMode::Linear);
        m_atlasTexture->SetWrapMode(GL::WrapMode::ClampToEdge);
        m_atlasTexture->UnBind();
        m_usingDistanceField = true;
    }
    else
    {
        loadedChars += "abcdefghijklmnopqrstuvwxyz";
        loadedChars += "ABCDEFGHIJKLMNOPQRSTUVWXYZ ";
        loadedChars += "0123456789()[]{}*.,;:-_=!<>+";
        loadedChars += "/\\$%&@\"'#¿?^";
        FontSheetCreator::LoadAtlasTexture(m_ttfFont, m_atlasTexture,
                                             loadedChars, &charPxRects);
        m_usingDistanceField = false;
    }

    for (int i = 0; i < loadedChars.Size(); ++i)
    {
        const char c = loadedChars[i];
        Recti charPxRect = charPxRects[i];
        Vector2 uvMin = Vector2(charPxRect.GetMin()) /
                             Vector2(m_atlasTexture->GetSize());
        Vector2 uvMax = Vector2(charPxRect.GetMax()) /
                             Vector2(m_atlasTexture->GetSize());
        m_charUvsInAtlas.Add(c, std::make_pair(uvMin, uvMax));
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

float Font::GetScaleProportion() const
{
    return GetMetricsSize() / 128.0f;
}

float Font::Scale(float magnitude) const
{
    return magnitude * GetScaleProportion();
}

Vector2 Font::Scale(const Vector2 &magnitude) const
{
    return magnitude * GetScaleProportion();
}

void Font::SetLoadSize(int loadSize) { m_ttfLoadSize = loadSize; }
void Font::SetMetricsSize(int metricsSize) { m_metricsSize = metricsSize; }

bool Font::IsUsingDistanceField() const { return m_usingDistanceField; }
int Font::GetLoadSize() const { return m_ttfLoadSize; }
int Font::GetMetricsSize() const { return m_metricsSize; }

Font::GlyphMetrics Font::GetCharMetrics(char c) const
{
    Font::GlyphMetrics charMetrics;
    if (!m_ttfFont) { return charMetrics; }

    int xmin, xmax, ymin, ymax, advance;
    TTF_GlyphMetrics(m_ttfFont, c, &xmin, &xmax, &ymin, &ymax, &advance);
    charMetrics.size    = Scale( Vector2((xmax - xmin), (ymax - ymin)) );
    charMetrics.bearing = Scale( Vector2(xmin, ymax) );
    charMetrics.advance = Scale( float(advance) );
    if (c == ' ')
    {
        charMetrics.size = Scale( Vector2(advance, GetLineSkip()) );
    }

    return charMetrics;
}

Vector2 Font::GetCharMinUvInAtlas(char c) const
{
    if (!m_charUvsInAtlas.ContainsKey(c)) { return Vector2::Zero; }
    return m_charUvsInAtlas.Get(c).first;
}

Vector2 Font::GetCharMaxUvInAtlas(char c) const
{
    if (!m_charUvsInAtlas.ContainsKey(c)) { return Vector2::Zero; }
    return m_charUvsInAtlas.Get(c).second;
}

bool Font::HasCharacter(char c) const
{
    return GetTTFFont() && TTF_GlyphIsProvided(GetTTFFont(), c);
}

Texture2D *Font::GetAtlasTexture() const
{
    return m_atlasTexture;
}

float Font::GetKerning(char leftChar, char rightChar) const
{
    if (!GetTTFFont() || !TTF_GetFontKerning(GetTTFFont())) { return -1; }
    #if (SDL_TTF_MAJOR_VERSION >= 2 && \
         SDL_TTF_MINOR_VERSION >= 0 && \
         SDL_TTF_PATCHLEVEL >= 14)
        return TTF_GetFontKerningSizeGlyphs(GetTTFFont(),
                                            leftChar, rightChar);
    #else
    return -1;
    #endif
}

float Font::GetLineSkip() const
{
    if (!GetTTFFont()) { return 0; }
    return Scale( float( TTF_FontLineSkip(GetTTFFont()) ) );
}

float Font::GetFontAscent() const
{
    if (!GetTTFFont()) { return 0; }
    return Scale( float( TTF_FontAscent(GetTTFFont()) ) );
}

float Font::GetFontDescent() const
{
    if (!GetTTFFont()) { return 0; }
    return Scale( float( TTF_FontDescent(GetTTFFont()) ) );
}

float Font::GetFontHeight() const
{
    if (!GetTTFFont()) { return 0; }
    return Scale( float( TTF_FontHeight(GetTTFFont()) ) );
}

Vector2i Font::GetSDFSpreadOffsetPx(char c) const
{
    if (!m_sdfSpreadOffsetPxInAtlas.ContainsKey(c)) { return Vector2i::Zero; }
    return m_sdfSpreadOffsetPxInAtlas.At(c);
}

TTF_Font *Font::GetTTFFont() const
{
    return m_ttfFont;
}

void Font::Free()
{
    m_charUvsInAtlas.Clear();
    if (m_atlasTexture) { Asset::Destroy(m_atlasTexture); }
    if (GetTTFFont()) { m_ttfFont = nullptr; TTF_CloseFont(GetTTFFont()); }
}
