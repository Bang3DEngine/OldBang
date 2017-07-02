#include "Bang/Font.h"

#include <GL/glew.h>

#include "Bang/Debug.h"
#include "Bang/XMLNode.h"
#include "Bang/Texture2D.h"
#include "Bang/FontSheetCreator.h"

Font::Font()
{
}

Font::~Font()
{
    Free();
}

String Font::GetFileExtensionStatic()
{
    return "bfont";
}

String Font::GetFileExtension() const
{
    return Font::GetFileExtensionStatic();
}

void Font::LoadFromTTF(const Path &filepath)
{
    Free();
    FontSheetCreator::LoadAtlasTexture(
                filepath,
                Font::c_charLoadSize,
                &m_atlasTexture,
                &m_charUvsInAtlas,
                &m_charMetrics,
                &m_freetypeFace);
}

void Font::Read(const XMLNode &xmlInfo)
{
    Asset::Read(xmlInfo);
    m_fontFilepath = xmlInfo.GetFilepath("FontFilepath");
    LoadFromTTF(m_fontFilepath);
}

void Font::Write(XMLNode *xmlInfo) const
{
    Asset::Write(xmlInfo);
    xmlInfo->SetFilepath("FontFilepath", m_fontFilepath,
                         Font::GetFileExtensionStatic());
}

Font::CharGlyphMetrics Font::GetCharacterMetrics(unsigned char c)
{
    if (!m_charMetrics.ContainsKey(c))
    {
        Font::CharGlyphMetrics cgm; // Empty
        return cgm;
    }
    return m_charMetrics.Get(c);
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

Texture2D *Font::GetAtlasTexture() const
{
    return m_atlasTexture;
}

int Font::GetKerningX(char leftChar, char rightChar)
{
    if (!FT_HAS_KERNING(m_freetypeFace)) { return -1; }

    FT_Vector kerning;
    int leftGlyphIndex  = FontSheetCreator::GetGlyphIndex(m_freetypeFace,
                                                          leftChar);
    int rightGlyphIndex = FontSheetCreator::GetGlyphIndex(m_freetypeFace,
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

void Font::Free()
{
    m_charMetrics.Clear();

    /*
    for (Texture2D* charTexture : m_charUvsInAtlas)
    {
        delete charTexture;
    }
    */
    m_charUvsInAtlas.Clear();
}
