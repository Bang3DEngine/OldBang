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

    xmlInfo->SetFilepath("MeshFilepath", m_meshFile.GetMeshFilepath(), "",
                         {XMLProperty::Readonly});
}
