#include "MeshAssetFileInspectable.h"

#include "XMLNode.h"

MeshAssetFileInspectable::MeshAssetFileInspectable(const MeshAssetFile &meshFile)
    : m_meshAssetFile(meshFile)
{
}

void MeshAssetFileInspectable::OnInspectorXMLChanged(const XMLNode *xmlInfo)
{
    m_meshAssetFile.SetMeshFilepath( xmlInfo->GetFilepath("MeshFilepath") );
}

void MeshAssetFileInspectable::OnInspectorXMLNeeded(XMLNode *xmlInfo) const
{
    xmlInfo->SetTagName("MeshAssetFileInspectable");

    xmlInfo->SetFilepath("MeshFilepath", m_meshAssetFile.GetMeshFilepath(),
                         "", {XMLProperty::Readonly});
}
