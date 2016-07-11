#ifndef INSPECTORTEXTURE2DWIDGET_H
#define INSPECTORTEXTURE2DWIDGET_H

#include <string>

#include "FileImage.h"
#include "InspectorWidget.h"

class InspectorImageFileWidget : public InspectorWidget
{
private:
    InspectorWidgetInfo m_inspectorInfo;

public:
    InspectorImageFileWidget(const FileImage &fileImage);
    virtual ~InspectorImageFileWidget();
};

#endif // INSPECTORTEXTURE2DWIDGET_H
