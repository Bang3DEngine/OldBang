#ifndef INSPECTORTEXTURE2DWIDGET_H
#define INSPECTORTEXTURE2DWIDGET_H

#include "InspectorWidget.h"

class InspectorTexture2DWidget : public InspectorWidget
{
private:
    InspectorWidgetInfo *inspectorInfo = nullptr;

public:
    InspectorTexture2DWidget(const std::string &filename);
    virtual ~InspectorTexture2DWidget();
};

#endif // INSPECTORTEXTURE2DWIDGET_H
