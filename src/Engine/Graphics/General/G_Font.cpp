#include "Bang/G_Font.h"

#include "Bang/Path.h"
#include "Bang/G_Texture2D.h"
#include "Bang/G_FontSheetCreator.h"

G_Font::G_Font()
{
}

G_Font::~G_Font()
{
    Free();
}

void G_Font::LoadFromTTF(const Path &ttfFilepath)
{
    Free();
    m_atlasTexture = new G_Texture2D();
    G_FontSheetCreator::LoadAtlasTexture(
        ttfFilepath,
        128,
        m_atlasTexture,
        &m_charUvsInAtlas,
        &m_charMetrics,
        &m_ttfFont);
}

G_Font::GlyphMetrics G_Font::GetCharacterMetrics(unsigned char c,
                                                     int textSize) const
{
    if (c == ' ') { return GetCharacterMetrics('A', textSize); }

    G_Font::GlyphMetrics cgm;
    if (m_charMetrics.ContainsKey(c)) { cgm = m_charMetrics.Get(c); }

    cgm.size     = SCAST<Vector2i>(ScaleMagnitude(Vector2f(cgm.size), textSize));
    cgm.bearing  = SCAST<Vector2i>(ScaleMagnitude(Vector2f(cgm.bearing), textSize));
    cgm.advance  = SCAST<int>(ScaleMagnitude(cgm.advance, textSize));

    return cgm;
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

G_Texture2D *G_Font::GetAtlasTexture() const
{
    return m_atlasTexture;
}

int G_Font::GetKerningXPx(char leftChar, char rightChar) const
{
    if (!m_ttfFont || !TTF_GetFontKerning(m_ttfFont)) { return -1; }
    return TTF_GetFontKerningSizeGlyphs(m_ttfFont, leftChar, rightChar);
}

int G_Font::GetLineSkipPx() const
{
    if (!m_ttfFont) { return 0; }
    return TTF_FontLineSkip(m_ttfFont);
}

void G_Font::Free()
{
    m_charMetrics.Clear();
    m_charUvsInAtlas.Clear();
    if (m_ttfFont) { TTF_CloseFont(m_ttfFont); }
}
