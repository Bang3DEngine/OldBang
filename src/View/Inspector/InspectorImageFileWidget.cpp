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

    m_inspectorInfo.GetStringSWSlotInfo("File name")->m_value =
            fileImage.GetName() + "." + fileImage.GetExtension();
    m_inspectorInfo.GetStringSWSlotInfo("Path")->m_value = fileImage.GetPath();
    m_inspectorInfo.GetStringSWSlotInfo("Dimensions")->m_value =
            std::to_string(fileImage.GetWidth()) + "x" + std::to_string(fileImage.GetHeight());
    m_inspectorInfo.GetStringSWSlotInfo("Mode")->m_value =
            fileImage.GetNumComponents() == 3 ? "RGB" : "RGBA";

    this->ConstructFromWidgetInformation(fileImage.GetName(), &m_inspectorInfo);
}

InspectorImageFileWidget::~InspectorImageFileWidget()
{
}
