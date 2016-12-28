#include "MaterialAssetFileInspectable.h"

#include "Scene.h"
#include "Material.h"
#include "Renderer.h"
#include "FileWriter.h"
#include "SceneManager.h"
#include "AssetsManager.h"

MaterialAssetFileInspectable::MaterialAssetFileInspectable
    (const MaterialAssetFile &materialAssetFile) :
        m_materialAssetFile(materialAssetFile)
{
    XMLNode *xmlMatInfo = XMLParser::FromFile(m_materialAssetFile.GetRelativePath());
    if (xmlMatInfo)
    {
        m_xmlInfo = *xmlMatInfo;
        delete xmlMatInfo;
    }
}

void MaterialAssetFileInspectable::OnInspectorXMLChanged(const XMLNode *xmlInfo)
{
    // Update live instances currently being used
    Material *currentMat = AssetsManager::Load<Material>(m_materialAssetFile.GetRelativePath());
    if (currentMat) // Now we update the asset file.
    {
        currentMat->OnInspectorXMLChanged(xmlInfo);
    }
    FileWriter::WriteToFile(m_materialAssetFile.GetRelativePath(), xmlInfo->ToString()); //Save
    m_xmlInfo = *xmlInfo;
}

void MaterialAssetFileInspectable::OnInspectorXMLNeeded(XMLNode *xmlInfo) const
{
    xmlInfo->SetTagName("MaterialAssetFileInspectable");

    *xmlInfo = m_xmlInfo;
}
