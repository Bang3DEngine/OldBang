#include "InspectorTexture2DWidget.h"

InspectorTexture2DWidget::
InspectorTexture2DWidget(const std::string &filename) :
    InspectorWidget()
{
    inspectorInfo = new InspectorWidgetInfo();
    inspectorInfo->SetSlotsInfos(
    {
        new InspectorAssetSWInfo("File", ".jpg, .png, .jpeg, .bmp"),
        new InspectorVFloatSWInfo("Dimensions", {16.0f, 16.0f})
    }
    );

    this->ConstructFromWidgetInformation(filename, inspectorInfo);
}

InspectorTexture2DWidget::~InspectorTexture2DWidget()
{
    if(inspectorInfo != nullptr)
        delete inspectorInfo;
}
