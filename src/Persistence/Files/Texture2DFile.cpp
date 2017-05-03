#include "Bang/Texture2DFile.h"

#include "Bang/IO.h"
#include "Bang/Debug.h"
#include "Bang/XMLParser.h"
#include "Bang/FileReader.h"
#include "Bang/IconManager.h"
#include "Bang/AssetsManager.h"
#include "Bang/FileInspectable.h"

Texture2DFile::Texture2DFile()
{
}

Texture2DFile::Texture2DFile(const Path& path)
    : File(path)
{
    // Load once and save the xmlInfo
    XMLNode xmlTexInfo = XMLParser::FromFile( GetPath() );
    m_xmlInfo = xmlTexInfo;
}

Path Texture2DFile::GetImageFilepath() const
{
    String contents = IO::GetFileContents(GetPath());
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
    m_xmlInfo = xmlInfo;

    #ifdef BANG_EDITOR
    AssetsManager::UpdateAsset(GetPath(), m_xmlInfo);
    #endif
}

void Texture2DFile::Write(XMLNode *xmlInfo) const
{
    File::Write(xmlInfo);
    *xmlInfo = m_xmlInfo;
}

#ifdef BANG_EDITOR
IInspectable *Texture2DFile::GetNewInspectable()
{
    return new FileInspectable<Texture2DFile>(*this);
}
#endif

bool Texture2DFile::IsAsset() const
{
    return true;
}
