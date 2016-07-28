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

    m_inspectorInfo.GetStringSWSlotInfo("File name")->m_value =
            fileMesh.GetName() + "." + fileMesh.GetExtension();
    m_inspectorInfo.GetStringSWSlotInfo("Path")->m_value = fileMesh.GetPath();
    m_inspectorInfo.GetStringSWSlotInfo("Mode")->m_value =
            fileMesh.IsTriangles() ? "Triangles" : "Quads";
    m_inspectorInfo.GetStringSWSlotInfo("Faces")->m_value =
            std::to_string(fileMesh.GetNumFaces());

    this->ConstructFromWidgetInformation(fileMesh.GetName(), &m_inspectorInfo);
}

InspectorMeshFileWidget::~InspectorMeshFileWidget()
{

}
