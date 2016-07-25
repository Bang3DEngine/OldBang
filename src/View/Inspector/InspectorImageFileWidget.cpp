#include "InspectorImageFileWidget.h"

InspectorImageFileWidget::
InspectorImageFileWidget(const FileImage &fileImage) :
    InspectorWidget()
{

    m_inspectorInfo.AddSlotInfos(
    {
        new InspectorStringSWInfo("File name", true),
        new InspectorStringSWInfo("Path", true),
        new InspectorStringSWInfo("Dimensions", true, false),
        new InspectorStringSWInfo("Mode", true, false)
    }
    );

    static_cast<InspectorStringSWInfo*>(m_inspectorInfo.GetSlotInfo("File name"))->m_value =
            fileImage.GetName() + "." + fileImage.GetExtension();

    static_cast<InspectorStringSWInfo*>(m_inspectorInfo.GetSlotInfo("Path"))->m_value =
            fileImage.GetPath();

    static_cast<InspectorStringSWInfo*>(m_inspectorInfo.GetSlotInfo("Dimensions"))->m_value =
            std::to_string(fileImage.GetWidth()) + "x" + std::to_string(fileImage.GetHeight());

    static_cast<InspectorStringSWInfo*>(m_inspectorInfo.GetSlotInfo("Mode"))->m_value =
            fileImage.GetNumComponents() == 3 ? "RGB" : "RGBA";

    this->ConstructFromWidgetInformation(fileImage.GetName(), &m_inspectorInfo);
}

InspectorImageFileWidget::~InspectorImageFileWidget()
{
}
