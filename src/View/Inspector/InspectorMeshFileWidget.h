#ifndef INSPECTORMESHFILEWIDGET_H
#define INSPECTORMESHFILEWIDGET_H

#include <string>

#include "FileMesh.h"
#include "InspectorWidget.h"

class InspectorMeshFileWidget: public IInspectable
{
private:
    FileMesh m_fileMesh;

public:
    InspectorMeshFileWidget(const FileMesh &fileMesh);

    virtual void OnInspectorXMLChanged(const XMLNode *xmlInfo);
    virtual void OnInspectorXMLNeeded(XMLNode *xmlInfo) const;
};

#endif // INSPECTORMESHFILEWIDGET_H
