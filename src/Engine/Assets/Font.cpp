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

void Font::LoadFromTTF(const Path &ttfFilepath)
{
    m_ttfFilepath = ttfFilepath;
    G_Font::LoadFromTTF(ttfFilepath);
}

Path Font::GetTTFFilepath() const
{
    return m_ttfFilepath;
}

void Font::Read(const XMLNode &xmlInfo)
{
    Asset::Read(xmlInfo);
    LoadFromTTF( xmlInfo.GetFilepath("FontFilepath") );
}

void Font::Write(XMLNode *xmlInfo) const
{
    Asset::Write(xmlInfo);
    xmlInfo->SetFilepath("FontFilepath", GetTTFFilepath());
}
