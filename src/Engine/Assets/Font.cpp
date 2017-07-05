#include "Bang/Font.h"

#include <GL/glew.h>

#include "Bang/Debug.h"
#include "Bang/XMLNode.h"
#include "Bang/Texture2D.h"

Font::Font()
{
}

Font::~Font()
{
}

String Font::GetFileExtensionStatic()
{
    return "bfont";
}

String Font::GetFileExtension() const
{
    return Font::GetFileExtensionStatic();
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
