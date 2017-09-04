#include "Bang/G_Font.h"

#include "Bang/Path.h"
#include "Bang/Rect.h"
#include "Bang/Vector2.h"
#include "Bang/XMLParser.h"
#include "Bang/G_Texture2D.h"
#include "Bang/G_FontSheetCreator.h"

G_Font::G_Font()
{
}

G_Font::~G_Font()
{
    Free();
}

void G_Font::Import(const Path &ttfFilepath)
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
    m_atlasTexture = new G_Texture2D();
    if (distFieldImgPath.IsFile() && distFieldInfoPath.IsFile())
    {
        G_Image distFieldImg;
        distFieldImg.Import(distFieldImgPath);
        m_atlasTexture->Import(distFieldImg);
        XMLNode distFieldInfo = XMLParser::FromFile(distFieldInfoPath);
        for (int i = 0; i < 255; ++i)
        {
            String attrName = "CharRect_" + String(i);
            if (distFieldInfo.Contains(attrName))
            {
                loadedChars.Append( String(char(i)) );
                Recti charPxRect = distFieldInfo.Get<Recti>(attrName);
                charPxRects.PushBack(charPxRect);
            }
        }
    }
    else
    {
        loadedChars += "abcdefghijklmnopqrstuvwxyz";
        loadedChars += "ABCDEFGHIJKLMNOPQRSTUVWXYZ ";
        loadedChars += "0123456789()[]{}*.,;:-_=!<>+";
        loadedChars += "/\\$%&@\"'#¿?^";
        G_FontSheetCreator::LoadAtlasTexture(m_ttfFont, m_atlasTexture,
                                             loadedChars, &charPxRects);
    }

    for (int i = 0; i < loadedChars.Size(); ++i)
    {
        const char c = loadedChars[i];
        Recti charPxRect = charPxRects[i];
        Vector2 uvMin = Vector2(charPxRect.GetMin()) /
                             Vector2(m_atlasTexture->GetSize());
        Vector2 uvMax = Vector2(charPxRect.GetMax()) /
                             Vector2(m_atlasTexture->GetSize());
        uvMin.y       = 1.0 - uvMin.y;
        uvMax.y       = 1.0 - uvMax.y;
        m_charUvsInAtlas.Add(c, std::make_pair(uvMin, uvMax));
    }
}

void G_Font::SetLoadSize(int loadSize)
{
    m_ttfLoadSize = loadSize;
}

const int G_Font::GetLoadSize() const
{
    return m_ttfLoadSize;
}

G_Font::GlyphMetrics G_Font::GetCharMetrics(unsigned char c, int fontSize) const
{
    G_Font::GlyphMetrics charMetrics;
    if (!m_ttfFont) { return charMetrics; }

    int xmin, xmax, ymin, ymax, advance;
    TTF_GlyphMetrics(m_ttfFont, c, &xmin, &xmax, &ymin, &ymax, &advance);
    charMetrics.size    = Vector2i((xmax - xmin), (ymax - ymin));
    charMetrics.bearing = Vector2i(xmin, ymax);
    charMetrics.advance = advance;

    charMetrics.size = SCAST<Vector2i>(
                                ScaleMagnitude(Vector2f(charMetrics.size),
                                                        fontSize));
    charMetrics.bearing = SCAST<Vector2i>(
                                ScaleMagnitude(Vector2f(charMetrics.bearing),
                                                        fontSize));
    charMetrics.advance = SCAST<int>(ScaleMagnitude(charMetrics.advance,
                                                    fontSize));

    return charMetrics;
}

Vector2 G_Font::GetCharMinUvInAtlas(char c) const
{
    if (!m_charUvsInAtlas.ContainsKey(c)) { return Vector2::Zero; }
    return m_charUvsInAtlas.Get(c).first;
}

Vector2 G_Font::GetCharMaxUvInAtlas(char c) const
{
    if (!m_charUvsInAtlas.ContainsKey(c)) { return Vector2::Zero; }
    return m_charUvsInAtlas.Get(c).second;
}

bool G_Font::HasCharacter(char c) const
{
    return GetTTFFont() && TTF_GlyphIsProvided(GetTTFFont(), c);
}

G_Texture2D *G_Font::GetAtlasTexture() const
{
    return m_atlasTexture;
}

int G_Font::GetKerningXPx(char leftChar, char rightChar) const
{
    if (!GetTTFFont() || !TTF_GetFontKerning(GetTTFFont())) { return -1; }
    return TTF_GetFontKerningSizeGlyphs(GetTTFFont(), leftChar, rightChar);
}

int G_Font::GetLineSkipPx() const
{
    if (!GetTTFFont()) { return 0; }
    return TTF_FontLineSkip(GetTTFFont());
}

TTF_Font *G_Font::GetTTFFont() const
{
    return m_ttfFont;
}

void G_Font::Free()
{
    m_charUvsInAtlas.Clear();
    if (GetTTFFont()) { m_ttfFont = nullptr; TTF_CloseFont(GetTTFFont()); }
}
