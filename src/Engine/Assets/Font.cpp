#include "Bang/Font.h"

#include <GL/glew.h>

#include "Bang/XMLNode.h"

Font::Font()
{
}

Font::~Font()
{
}

void Font::Import(const Path &ttfFilepath)
{
    m_ttfFilepath = ttfFilepath;
    G_Font::LoadFromTTF(ttfFilepath);
}

Path Font::GetTTFFilepath() const
{
    return m_ttfFilepath;
}

// SerializableObject
void Font::ImportXML(const XMLNode &xmlInfo)
{
    Asset::ImportXML(xmlInfo);
}

void Font::ExportXML(XMLNode *xmlInfo) const
{
    Asset::ExportXML(xmlInfo);
}
