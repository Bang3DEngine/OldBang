#include "MaterialAssetFileInspectable.h"

#include "Scene.h"
#include "Material.h"
#include "Renderer.h"
#include "IconManager.h"
#include "SceneManager.h"
#include "AssetsManager.h"

MaterialAssetFileInspectable::MaterialAssetFileInspectable
    (const MaterialAssetFile &materialAssetFile) :
        m_materialAssetFile(materialAssetFile)
{
    XMLNode *xmlMatInfo = XMLParser::FromFile(
                m_materialAssetFile.GetRelativePath());
    if (xmlMatInfo)
    {
        m_xmlInfo = *xmlMatInfo;
        delete xmlMatInfo;
    }
}

const QPixmap &MaterialAssetFileInspectable::GetIcon() const
{
    return Material::GetIconStatic();
}

void MaterialAssetFileInspectable::Read(const XMLNode &xmlInfo)
{
    // Update live instances currently being used
    AssetsManager::OnAssetFileChanged<Material>(
                m_materialAssetFile.GetAbsolutePath(), xmlInfo);
    m_xmlInfo = xmlInfo;
}

void MaterialAssetFileInspectable::Write(XMLNode *xmlInfo) const
{
    xmlInfo->SetTagName("MaterialAssetFileInspectable");
    *xmlInfo = m_xmlInfo;

    IconManager::InvalidatePixmap( m_materialAssetFile.GetAbsolutePath() );
}
