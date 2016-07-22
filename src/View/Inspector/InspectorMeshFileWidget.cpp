#include "InspectorMeshFileWidget.h"

InspectorMeshFileWidget::InspectorMeshFileWidget(const FileMesh &fileMesh) :
    InspectorWidget()
{
    m_inspectorInfo.AddSlotInfos(
    {
        new InspectorStringSWInfo("File name",
                    fileMesh.GetName() + "." + fileMesh.GetExtension(), true),
        new InspectorStringSWInfo("Path", fileMesh.GetPath(), true),
        new InspectorStringSWInfo("Mode",
                    fileMesh.IsTriangles() ? "Triangles" : "Quads", true),
        new InspectorStringSWInfo("Faces",
                    std::to_string(fileMesh.GetNumFaces()), true)

    }
    );

    this->ConstructFromWidgetInformation(fileMesh.GetName(), &m_inspectorInfo);
}

InspectorMeshFileWidget::~InspectorMeshFileWidget()
{

}
