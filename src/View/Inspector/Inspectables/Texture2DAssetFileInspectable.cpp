#include "Bang/Texture2DAssetFileInspectable.h"

#include "Bang/IO.h"
#include "Bang/Texture2D.h"
#include "Bang/AssetsManager.h"

Texture2DAssetFileInspectable::Texture2DAssetFileInspectable()
{
}

Texture2DAssetFileInspectable::Texture2DAssetFileInspectable
    (const Texture2DAssetFile &fileTex) :
    m_fileTex(fileTex)
{
    // Load once and save the xmlInfo
    XMLNode *xmlTexInfo = XMLParser::FromFile(m_fileTex.GetRelativePath());
    if (xmlTexInfo)
    {
        // We can do this safely, xmlTexInfo wont have children.
        m_xmlInfo = *xmlTexInfo;
        delete xmlTexInfo;
    }
}

const QPixmap &Texture2DAssetFileInspectable::GetIcon() const
{
    return Texture2D::GetIconStatic();
}

void Texture2DAssetFileInspectable::Read(const XMLNode &xmlInfo)
{
    // Update live instances currently being used
    AssetsManager::UpdateAsset(m_fileTex.GetAbsolutePath(), xmlInfo);
    m_xmlInfo = xmlInfo;
}

void Texture2DAssetFileInspectable::Write(XMLNode *xmlInfo) const
{
    xmlInfo->SetTagName("Texture2DAssetFileInspectable");
    *xmlInfo = m_xmlInfo;
}
