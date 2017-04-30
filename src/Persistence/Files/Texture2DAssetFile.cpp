#include "Bang/Texture2DAssetFile.h"

#include "Bang/IO.h"
#include "Bang/Debug.h"
#include "Bang/XMLParser.h"
#include "Bang/FileReader.h"
#include "Bang/IconManager.h"
#include "Bang/AssetsManager.h"
#include "Bang/FileInspectable.h"

Texture2DAssetFile::Texture2DAssetFile()
{
}

Texture2DAssetFile::Texture2DAssetFile(const Path& path)
    : File(path)
{
    // Load once and save the xmlInfo
    XMLNode xmlTexInfo = XMLParser::FromFile(GetPath().GetAbsolute());
    m_xmlInfo = xmlTexInfo;
}

Path Texture2DAssetFile::GetImageFilepath() const
{
    String contents = IO::GetFileContents(GetPath().GetAbsolute());
    XMLNode xmlInfo = XMLNode::FromString(contents);
    return xmlInfo.GetFilepath("ImageFilepath");
}

const QPixmap& Texture2DAssetFile::GetIcon() const
{
    return IconManager::LoadPixmap(GetImageFilepath(),
                                   IconManager::IconOverlay::Asset);
}

void Texture2DAssetFile::Read(const XMLNode &xmlInfo)
{
    m_xmlInfo = xmlInfo;
    AssetsManager::UpdateAsset(GetPath(), m_xmlInfo);
}

void Texture2DAssetFile::Write(XMLNode *xmlInfo) const
{
    File::Write(xmlInfo);
    *xmlInfo = m_xmlInfo;
}

#ifdef BANG_EDITOR
IInspectable *Texture2DAssetFile::GetNewInspectable()
{
    return new FileInspectable<Texture2DAssetFile>(*this);
}
#endif

bool Texture2DAssetFile::IsAsset() const
{
    return true;
}
