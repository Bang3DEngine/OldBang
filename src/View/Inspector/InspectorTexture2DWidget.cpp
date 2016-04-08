#include "InspectorTexture2DWidget.h"

InspectorTexture2DWidget::
InspectorTexture2DWidget(const FileImage &fileImage) :
    InspectorWidget()
{
    std::string mode = fileImage.GetNumComponents() == 3 ?
                "RGB" : "RGBA";

    inspectorInfo.SetSlotsInfos(
    {
        new InspectorAssetSWInfo("File", ".jpg, .png, .jpeg, .bmp"),
        new InspectorVFloatSWInfo("Dimensions",
                    {fileImage.GetWidth(),
                     fileImage.GetHeight()}),
        new InspectorEnumSWInfo("Mode", {mode}),
        new InspectorEnumSWInfo("File name", {fileImage.GetName()}),
        new InspectorEnumSWInfo("Extension", {fileImage.GetExtension()}),
        new InspectorEnumSWInfo("Path", {fileImage.GetPath()})
    }
    );

    this->ConstructFromWidgetInformation(fileImage.GetName(), &inspectorInfo);
}

InspectorTexture2DWidget::~InspectorTexture2DWidget()
{
}
