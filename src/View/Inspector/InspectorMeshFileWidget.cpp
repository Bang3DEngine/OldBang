#include "InspectorMeshFileWidget.h"

InspectorMeshFileWidget::InspectorMeshFileWidget(const FileMesh &fileMesh) :
    m_fileMesh(fileMesh)
{
}

void InspectorMeshFileWidget::OnInspectorXMLChanged(const XMLNode *xmlInfo)
{
    // They all are readonly properties, so we wont do anything here
}

void InspectorMeshFileWidget::OnInspectorXMLNeeded(XMLNode *xmlInfo) const
{
    xmlInfo->SetString("FileName", m_fileMesh.GetName() + "." + m_fileMesh.GetExtension(),
                      {XMLProperty::Readonly});
    xmlInfo->SetString("Path", m_fileMesh.GetPath(), {XMLProperty::Readonly});
    xmlInfo->SetString("Mode", m_fileMesh.IsTriangles() ? "Triangles" : "Quads",
                      {XMLProperty::Readonly});
    xmlInfo->SetString("Faces",  std::to_string(m_fileMesh.GetNumFaces()),
                      {XMLProperty::Readonly});
}
