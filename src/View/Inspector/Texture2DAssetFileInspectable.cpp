#include "Texture2DAssetFileInspectable.h"

#include "Texture2D.h"
#include "Persistence.h"
#include "AssetsManager.h"

Texture2DAssetFileInspectable::Texture2DAssetFileInspectable
    (const Texture2DAssetFile &fileTex) :
    m_fileTex(fileTex)
{
    // Load once and save the xmlInfo
    XMLNode *xmlTexInfo = XMLParser::FromFile(m_fileTex.GetPath());
    if (xmlTexInfo)
    {
        m_xmlInfo = *xmlTexInfo; // We can do this safely, xmlTexInfo wont have children.
        delete xmlTexInfo;
    }
}

void Texture2DAssetFileInspectable::OnInspectorXMLChanged(const XMLNode *xmlInfo)
{
    // Update live instances currently being used
    Texture2D *currentTex = AssetsManager::GetCachedAsset<Texture2D>(m_fileTex.GetPath());
    if (currentTex) // Now we update the asset file.
    {
        currentTex->OnInspectorXMLChanged(xmlInfo);
    }
    FileWriter::WriteToFile(m_fileTex.GetPath(), xmlInfo->ToString()); //Save
    m_xmlInfo = *xmlInfo;
}

void Texture2DAssetFileInspectable::OnInspectorXMLNeeded(XMLNode *xmlInfo) const
{
    *xmlInfo = m_xmlInfo;
}
