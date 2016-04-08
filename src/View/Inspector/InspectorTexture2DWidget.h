#ifndef INSPECTORTEXTURE2DWIDGET_H
#define INSPECTORTEXTURE2DWIDGET_H

#include "FileImage.h"
#include "InspectorWidget.h"

class InspectorTexture2DWidget : public InspectorWidget
{
private:
    InspectorWidgetInfo inspectorInfo;

public:
    InspectorTexture2DWidget(const FileImage &fileImage);
    virtual ~InspectorTexture2DWidget();
};

#endif // INSPECTORTEXTURE2DWIDGET_H
