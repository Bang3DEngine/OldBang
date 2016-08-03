#include "MeshAssetFileInspectable.h"

MeshAssetFileInspectable::MeshAssetFileInspectable(const MeshAssetFile &meshFile)
    : m_meshFile(meshFile)
{
}

void MeshAssetFileInspectable::OnInspectorXMLChanged(const XMLNode *xmlInfo)
{

}

void MeshAssetFileInspectable::OnInspectorXMLNeeded(XMLNode *xmlInfo) const
{
    xmlInfo->SetTagName("MeshAssetFileInspectable");

    xmlInfo->SetString("MeshFilepath", m_meshFile.GetMeshFilepath(),
                       {XMLProperty::Readonly});
}
