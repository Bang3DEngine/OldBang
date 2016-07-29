#include "InspectorMeshFileWidget.h"

InspectorMeshFileWidget::InspectorMeshFileWidget(const FileMesh &fileMesh) :
    InspectorWidget()
{
    xmlInfo->SetString("FileName", fileMesh.GetName() + "." + fileMesh.GetExtension(),
                       {XMLProperty::Readonly});
    xmlInfo->SetString("Path", fileMesh.GetPath(), {XMLProperty::Readonly});
    xmlInfo->SetString("Mode", fileMesh.IsTriangles() ? "Triangles" : "Quads",
                       {XMLProperty::Readonly});
    xmlInfo->SetString("Faces",  std::to_string(fileMesh.GetNumFaces()),
                       {XMLProperty::Readonly});

    ConstructFromWidgetXMLInfo(fileMesh.GetName(), *xmlInfo);
}

InspectorMeshFileWidget::~InspectorMeshFileWidget()
{

}
