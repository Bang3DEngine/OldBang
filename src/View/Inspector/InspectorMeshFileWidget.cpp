#include "InspectorMeshFileWidget.h"

InspectorMeshFileWidget::InspectorMeshFileWidget(const FileMesh &fileMesh) :
    InspectorWidget()
{
    m_inspectorInfo.AddSlotInfos(
    {
        new InspectorStringSWInfo("File name", true),
        new InspectorStringSWInfo("Path", true),
        new InspectorStringSWInfo("Mode", true),
        new InspectorStringSWInfo("Faces", true)

    }
    );

    static_cast<InspectorStringSWInfo*>(m_inspectorInfo.GetSlotInfo("File name"))->m_value =
            fileMesh.GetName() + "." + fileMesh.GetExtension();

    static_cast<InspectorStringSWInfo*>(m_inspectorInfo.GetSlotInfo("Path"))->m_value =
            fileMesh.GetPath();

    static_cast<InspectorStringSWInfo*>(m_inspectorInfo.GetSlotInfo("Mode"))->m_value =
            fileMesh.IsTriangles() ? "Triangles" : "Quads";

    static_cast<InspectorStringSWInfo*>(m_inspectorInfo.GetSlotInfo("Faces"))->m_value =
            std::to_string(fileMesh.GetNumFaces());

    this->ConstructFromWidgetInformation(fileMesh.GetName(), &m_inspectorInfo);
}

InspectorMeshFileWidget::~InspectorMeshFileWidget()
{

}
