#include "InspectorTexture2DWidget.h"

InspectorTexture2DWidget::
InspectorTexture2DWidget(const FileImage &fileImage) :
    InspectorWidget()
{
    std::string mode = fileImage.GetNumComponents() == 3 ?
                "RGB" : "RGBA";

    inspectorInfo.SetSlotsInfos(
    {
        new InspectorStringSWInfo("Dimensions",
                     std::to_string(fileImage.GetWidth()) +
                     "x" + std::to_string(fileImage.GetHeight()),
                     true, false
                    ),
        new InspectorStringSWInfo("Mode", mode, true, false),
        new InspectorStringSWInfo("File name",
                    fileImage.GetName() + "." + fileImage.GetExtension(), true),
        new InspectorStringSWInfo("Path", fileImage.GetPath(), true)
    }
    );

    this->ConstructFromWidgetInformation(fileImage.GetName(), &inspectorInfo);
}

InspectorTexture2DWidget::~InspectorTexture2DWidget()
{
}
