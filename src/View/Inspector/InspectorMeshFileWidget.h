#ifndef INSPECTORMESHFILEWIDGET_H
#define INSPECTORMESHFILEWIDGET_H

#include <string>

#include "FileMesh.h"
#include "InspectorWidget.h"

class InspectorMeshFileWidget: public InspectorWidget
{
private:
    XMLNode *xmlInfo;

public:
    InspectorMeshFileWidget(const FileMesh &fileMesh);
    virtual ~InspectorMeshFileWidget();
};

#endif // INSPECTORMESHFILEWIDGET_H
