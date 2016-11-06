#include "Font.h"

#include "FontSheetCreator.h"

Font::Font()
{
}

Font::~Font()
{
}

void Font::LoadFromFile(const String &m_filepath)
{
    m_fontsheet = FontSheetCreator::CreateFontSheet(m_filepath, 512);
    if (m_fontsheet)
    {
        m_fontsheet->SetWrapMode(Texture::WrapMode::Clamp);
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

Texture2D *Font::GetFontSheet() const
{
    return m_fontsheet;
}
