#include "MaterialAssetFileInspectable.h"

MaterialAssetFileInspectable::MaterialAssetFileInspectable
    (const MaterialAssetFile &materialAssetFile) :
        m_materialAssetFile(materialAssetFile)
{
    XMLNode *xmlMatInfo = XMLParser::FromFile(m_materialAssetFile.GetPath());
    if (xmlMatInfo)
    {
        m_xmlInfo = *xmlMatInfo;
        delete xmlMatInfo;
    }
}

void MaterialAssetFileInspectable::OnInspectorXMLChanged(const XMLNode *xmlInfo)
{
    // Update live instances currently being used
    Material *currentMat = AssetsManager::LoadAsset<Material>(m_materialAssetFile.GetPath());
    if (currentMat) // Now we update the asset file.
    {
        currentMat->OnInspectorXMLChanged(xmlInfo);
    }
    FileWriter::WriteToFile(m_materialAssetFile.GetPath(), xmlInfo->ToString()); //Save
    m_xmlInfo = *xmlInfo;
}

void MaterialAssetFileInspectable::OnInspectorXMLNeeded(XMLNode *xmlInfo) const
{
    *xmlInfo = m_xmlInfo;
}
