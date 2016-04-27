#include "InspectorTexture2DAssetWidget.h"

InspectorTexture2DAssetWidget::InspectorTexture2DAssetWidget
    (const FileTexture2DAsset &fileTex) : InspectorWidget()
{
    std::string mode = fileTex.GetNumComponents() == 3 ?
                "RGB" : "RGBA";

    inspectorInfo.SetSlotsInfos(
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

    this->ConstructFromWidgetInformation(fileImage.GetName(), &inspectorInfo);
