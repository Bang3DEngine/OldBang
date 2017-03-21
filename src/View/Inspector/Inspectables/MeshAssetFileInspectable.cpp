#include "MeshAssetFileInspectable.h"

#include "Mesh.h"
#include "XMLNode.h"
#include "AssetsManager.h"

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
    AssetsManager::OnAssetFileChanged<Mesh>(m_meshAssetFile.GetAbsolutePath(),
                                            xmlInfo);
}

void MeshAssetFileInspectable::Write(XMLNode *xmlInfo) const
{
    xmlInfo->SetTagName("MeshAssetFileInspectable");
    xmlInfo->SetFilepath("MeshFilepath", m_meshAssetFile.GetMeshFilepath(),
                         "obj");
}
