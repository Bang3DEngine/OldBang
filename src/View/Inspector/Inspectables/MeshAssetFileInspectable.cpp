#include "Bang/MeshAssetFileInspectable.h"

#include "Bang/Mesh.h"
#include "Bang/XMLNode.h"
#include "Bang/AssetsManager.h"

MeshAssetFileInspectable::MeshAssetFileInspectable(
        const MeshAssetFile &meshFile)
    : m_meshAssetFile(meshFile)
{
}

const QPixmap &MeshAssetFileInspectable::GetIcon() const
{
    return Mesh::GetIconStatic();
}

void MeshAssetFileInspectable::Read(const XMLNode &xmlInfo)
{
    m_meshAssetFile.SetMeshFilepath( xmlInfo.GetFilepath("MeshFilepath") );
    AssetsManager::UpdateAsset(m_meshAssetFile.GetAbsolutePath(), xmlInfo);
}

void MeshAssetFileInspectable::Write(XMLNode *xmlInfo) const
{
    xmlInfo->SetTagName("MeshAssetFileInspectable");
    xmlInfo->SetFilepath("MeshFilepath", m_meshAssetFile.GetMeshFilepath(),
                         "obj");
}
