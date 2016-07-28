#include "InspectorMeshFileWidget.h"

InspectorMeshFileWidget::InspectorMeshFileWidget(const FileMesh &fileMesh) :
    InspectorWidget()
{
    xmlInfo->SetAttribute("FileName", fileMesh.GetName() + "." + fileMesh.GetExtension());
    xmlInfo->SetAttribute("Path", fileMesh.GetPath());
    xmlInfo->SetAttribute("Mode", fileMesh.IsTriangles() ? "Triangles" : "Quads");
    xmlInfo->SetAttribute("Faces",  std::to_string(fileMesh.GetNumFaces()));

    ConstructFromWidgetXMLInfo(fileMesh.GetName(), *xmlInfo);
}

InspectorMeshFileWidget::~InspectorMeshFileWidget()
{

}
