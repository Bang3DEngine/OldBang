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

void G_Font::LoadFromTTF(const Path &filepath)
{
    Free();
    G_FontSheetCreator::LoadAtlasTexture(
        filepath,
        G_Font::c_charLoadSize,
        &m_atlasTexture,
        &m_charUvsInAtlas,
        &m_charMetrics,
        &m_freetypeFace);
}

G_Font::CharGlyphMetrics G_Font::GetCharacterMetrics(unsigned char c,
                                                     int textSize) const
{
    if (c == ' ') { return GetCharacterMetrics('A', textSize); }

    G_Font::CharGlyphMetrics cgm;
    if (m_charMetrics.ContainsKey(c)) { cgm = m_charMetrics.Get(c); }

    cgm.size    = SCAST<Vector2i>(ScaleMagnitude(Vector2f(cgm.size), textSize));
    cgm.bearing = SCAST<Vector2i>(ScaleMagnitude(Vector2f(cgm.bearing), textSize));
    cgm.advance = SCAST<int>(ScaleMagnitude(cgm.advance, textSize));
    cgm.originY = SCAST<int>(ScaleMagnitude(cgm.originY, textSize));

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

int G_Font::GetKerningX(char leftChar, char rightChar)
{
    if (!FT_HAS_KERNING(m_freetypeFace)) { return -1; }

    FT_Vector kerning;
    int leftGlyphIndex  = G_FontSheetCreator::GetGlyphIndex(m_freetypeFace,
                                                            leftChar);
    int rightGlyphIndex = G_FontSheetCreator::GetGlyphIndex(m_freetypeFace,
                                                            rightChar);
    int error = FT_Get_Kerning(m_freetypeFace,
                               leftGlyphIndex, rightGlyphIndex,
                               FT_KERNING_DEFAULT, &kerning);
    if (!error)
    {
        return kerning.x / 64;
    }
    return -1;
}

void G_Font::Free()
{
    m_charMetrics.Clear();
    m_charUvsInAtlas.Clear();
}
