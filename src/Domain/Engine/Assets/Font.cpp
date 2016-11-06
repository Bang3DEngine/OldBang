#include "Font.h"

#include "FontSheetCreator.h"

int Font::CharLoadSize = 64;

Font::Font()
{
}

Font::~Font()
{
    for (Texture2D* charTexture : m_charTextures)
    {
        delete charTexture;
    }
}

void Font::LoadFromFile(const String &m_filepath)
{
    for (int c = 0; c <= 255; ++c)
    {
        Texture2D *charTexture = FontSheetCreator::CreateCharTexture(m_filepath,
                                                                     Font::CharLoadSize,
                                                                     char(c));
        m_charTextures.Add(charTexture);
    }
}

void Font::ReadXMLInfo(const XMLNode *xmlInfo)
{
    Asset::ReadXMLInfo(xmlInfo);
    m_filepath = xmlInfo->GetFilepath("FontFilepath");
    LoadFromFile(m_filepath);
}

void Font::FillXMLInfo(XMLNode *xmlInfo) const
{
    Asset::FillXMLInfo(xmlInfo);
    xmlInfo->SetTagName("Font");
    xmlInfo->SetFilepath("FontFilepath", m_filepath, Font::GetFileExtensionStatic());
}

Texture2D *Font::GetCharacterTexture(unsigned char c) const
{
    return m_charTextures[c];
}
