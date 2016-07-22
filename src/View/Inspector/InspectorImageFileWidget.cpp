#include "InspectorImageFileWidget.h"

InspectorImageFileWidget::
InspectorImageFileWidget(const FileImage &fileImage) :
    InspectorWidget()
{
    std::string mode = fileImage.GetNumComponents() == 3 ?
                "RGB" : "RGBA";

    m_inspectorInfo.AddSlotInfos(
    {
        new InspectorStringSWInfo("File name",
                    fileImage.GetName() + "." + fileImage.GetExtension(), true),
        new InspectorStringSWInfo("Path", fileImage.GetPath(), true),
        new InspectorStringSWInfo("Dimensions",
                     std::to_string(fileImage.GetWidth()) +
                     "x" + std::to_string(fileImage.GetHeight()),
                     true, false
                    ),
        new InspectorStringSWInfo("Mode", mode, true, false)
    }
    );

    this->ConstructFromWidgetInformation(fileImage.GetName(), &m_inspectorInfo);
}

InspectorImageFileWidget::~InspectorImageFileWidget()
{
}
