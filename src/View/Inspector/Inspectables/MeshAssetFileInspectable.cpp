#include "MeshAssetFileInspectable.h"

#include "Mesh.h"
#include "XMLNode.h"
#include "AssetsManager.h"

MeshAssetFileInspectable::MeshAssetFileInspectable(
        const MeshAssetFile &meshFile)
    : m_meshAssetFile(meshFile)
{
}

void MeshAssetFileInspectable::OnInspectorXMLChanged(const XMLNode *xmlInfo)
{
    m_meshAssetFile.SetMeshFilepath( xmlInfo->GetFilepath("MeshFilepath") );
    AssetsManager::OnAssetFileChanged<Mesh>(m_meshAssetFile.GetAbsolutePath(),
                                            xmlInfo);
}

void MeshAssetFileInspectable::OnInspectorXMLNeeded(XMLNode *xmlInfo) const
{
    xmlInfo->SetTagName("MeshAssetFileInspectable");
    xmlInfo->SetFilepath("MeshFilepath", m_meshAssetFile.GetMeshFilepath(),
                         "obj");
}
