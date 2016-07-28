#include "InspectorMeshFileWidget.h"

InspectorMeshFileWidget::InspectorMeshFileWidget(const FileMesh &fileMesh) :
    InspectorWidget()
{
    xmlInfo->SetString("FileName", fileMesh.GetName() + "." + fileMesh.GetExtension());
    xmlInfo->SetString("Path", fileMesh.GetPath());
    xmlInfo->SetString("Mode", fileMesh.IsTriangles() ? "Triangles" : "Quads");
    xmlInfo->SetString("Faces",  std::to_string(fileMesh.GetNumFaces()));

    ConstructFromWidgetXMLInfo(fileMesh.GetName(), *xmlInfo);
}

InspectorMeshFileWidget::~InspectorMeshFileWidget()
{

}
