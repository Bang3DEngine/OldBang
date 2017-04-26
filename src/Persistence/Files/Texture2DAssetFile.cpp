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

Texture2DAssetFile::Texture2DAssetFile(const QFileSystemModel *model,
                                       const QModelIndex &index)
    : File(model, index)
{
    // Load once and save the xmlInfo
    XMLNode *xmlTexInfo = XMLParser::FromFile(GetRelativePath());
    if (xmlTexInfo)
    {
        // We can do this safely, xmlTexInfo wont have children.
        m_xmlInfo = *xmlTexInfo;
        delete xmlTexInfo;
    }
}

String Texture2DAssetFile::GetImageAbsFilepath() const
{
    String contents = IO::GetFileContents(GetAbsolutePath());
    XMLNode *xmlInfo = XMLNode::FromString(contents);
    String result = "";
    if (xmlInfo)
    {
        result = xmlInfo->GetFilepath("ImageFilepath");
        delete xmlInfo;
    }
    return result;
}

const QPixmap& Texture2DAssetFile::GetIcon() const
{
    return IconManager::LoadPixmap(GetImageAbsFilepath(),
                                   IconManager::IconOverlay::Asset);
}

void Texture2DAssetFile::Read(const XMLNode &xmlInfo)
{
    // Update live instances currently being used
    AssetsManager::UpdateAsset(GetAbsolutePath(), xmlInfo);
    m_xmlInfo = xmlInfo;
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
