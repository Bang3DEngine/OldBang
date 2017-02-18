#include "Font.h"

#include <GL/glew.h>

#include "Debug.h"
#include "XMLNode.h"
#include "Texture2D.h"
#include "FontSheetCreator.h"

int Font::CharLoadSize = 64;

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

String Font::GetFileExtension()
{
    return Font::GetFileExtensionStatic();
}

void Font::LoadFromTTF(const String &m_filepath)
{
    Free();
    for (int c = 0; c <= 255; ++c)
    {
        Texture2D *charTexture = nullptr;
        CharGlyphMetrics charMetrics;
        if (FontSheetCreator::LoadCharTexture(
                    m_filepath, Font::CharLoadSize, char(c),
                    &charTexture, &charMetrics, &m_freetypeFace))
        {
            charTexture->SetFilterMode(Texture2D::FilterMode::Trilinear);
            m_charMetrics.Add(charMetrics);
            m_charTextures.Add(charTexture);
        }
    }
}

void Font::ReadXMLInfo(const XMLNode *xmlInfo)
{
    Asset::ReadXMLInfo(xmlInfo);
    m_fontFilepath = xmlInfo->GetFilepath("FontFilepath");
    LoadFromTTF(m_fontFilepath);
}

void Font::FillXMLInfo(XMLNode *xmlInfo) const
{
    Asset::FillXMLInfo(xmlInfo);
    xmlInfo->SetTagName("Font");
    xmlInfo->SetFilepath("FontFilepath", m_fontFilepath, Font::GetFileExtensionStatic());
}

const Font::CharGlyphMetrics &Font::GetCharacterMetrics(unsigned char c)
{
    return m_charMetrics[c];
}

Texture2D *Font::GetCharacterTexture(unsigned char c) const
{
    return m_charTextures[c];
}

int Font::GetKerningX(char leftChar, char rightChar)
{
    if (!FT_HAS_KERNING(m_freetypeFace)) { return -1; }

    FT_Vector kerning;
    int leftGlyphIndex  = FontSheetCreator::GetGlyphIndex(m_freetypeFace,  leftChar);
    int rightGlyphIndex = FontSheetCreator::GetGlyphIndex(m_freetypeFace, rightChar);
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

    for (Texture2D* charTexture : m_charTextures)
    {
        delete charTexture;
    }
    m_charTextures.Clear();
}
