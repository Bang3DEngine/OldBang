#ifndef INSPECTORTEXTURE2DASSETWIDGET_H
#define INSPECTORTEXTURE2DASSETWIDGET_H

#include <string>

#include "FileTexture2DAsset.h"
#include "InspectorWidget.h"

class InspectorTexture2DAssetWidget : public InspectorWidget
{
private:
    InspectorWidgetInfo inspectorInfo;

public:
    InspectorTexture2DAssetWidget(const FileTexture2DAsset &fileTex);
    virtual ~InspectorTexture2DAssetWidget();
};

#endif // INSPECTORTEXTURE2DASSETWIDGET_H
