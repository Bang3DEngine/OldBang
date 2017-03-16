#include "Texture2DAssetFileInspectable.h"

#include "Texture2D.h"
#include "IO.h"
#include "AssetsManager.h"

Texture2DAssetFileInspectable::Texture2DAssetFileInspectable
    (const Texture2DAssetFile &fileTex) :
    m_fileTex(fileTex)
{
    // Load once and save the xmlInfo
    XMLNode *xmlTexInfo = XMLParser::FromFile(m_fileTex.GetRelativePath());
    if (xmlTexInfo)
    {
        m_xmlInfo = *xmlTexInfo; // We can do this safely, xmlTexInfo wont have children.
        delete xmlTexInfo;
    }
}

void Texture2DAssetFileInspectable::Read(const XMLNode &xmlInfo)
{
    // Update live instances currently being used
    AssetsManager::OnAssetFileChanged<Texture2D>(m_fileTex.GetAbsolutePath(),
                                                 xmlInfo);
    m_xmlInfo = xmlInfo;
}

void Texture2DAssetFileInspectable::Write(XMLNode *xmlInfo) const
{
    xmlInfo->SetTagName("Texture2DAssetFileInspectable");
    *xmlInfo = m_xmlInfo;
}
