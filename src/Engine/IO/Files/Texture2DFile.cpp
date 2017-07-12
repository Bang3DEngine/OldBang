#include "Bang/Texture2DFile.h"

#include "Bang/Paths.h"
#include "Bang/Debug.h"
#include "Bang/XMLParser.h"
#include "Bang/FileReader.h"
#include "Bang/IconManager.h"
#include "Bang/AssetsManager.h"

Texture2DFile::Texture2DFile()
{
}

Texture2DFile::Texture2DFile(const Path& path)
    : BFile(path)
{
    // Load once and save the xmlInfo
    XMLNode xmlTexInfo = XMLParser::FromFile( GetPath() );
    m_xmlInfo = xmlTexInfo;
}

Path Texture2DFile::GetImageFilepath() const
{
    String contents = File::GetContents(GetPath());
    XMLNode xmlInfo = XMLNode::FromString(contents);
    return xmlInfo.GetFilepath("ImageFilepath");
}

const QPixmap& Texture2DFile::GetIcon() const
{
    return IconManager::LoadPixmap(GetImageFilepath(),
                                   IconManager::IconOverlay::Asset);
}

void Texture2DFile::Read(const XMLNode &xmlInfo)
{
    BFile::Read(xmlInfo);
    m_xmlInfo = xmlInfo;

    #ifdef BANG_EDITOR
    AssetsManager::UpdateAsset(GetPath(), m_xmlInfo);
    #endif
}

void Texture2DFile::Write(XMLNode *xmlInfo) const
{
    BFile::Write(xmlInfo);
    *xmlInfo = m_xmlInfo;
}

bool Texture2DFile::IsAsset() const
{
    return true;
}
