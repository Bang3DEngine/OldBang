#ifndef INSPECTORMESHFILEWIDGET_H
#define INSPECTORMESHFILEWIDGET_H

#include <string>

#include "MeshFile.h"
#include "InspectorWidget.h"

class MeshFileInspectable: public IInspectable
{
private:
    MeshFile m_fileMesh;

public:
    MeshFileInspectable(const MeshFile &fileMesh);

    virtual void OnInspectorXMLChanged(const XMLNode *xmlInfo) override;
    virtual void OnInspectorXMLNeeded(XMLNode *xmlInfo) const override;
};

#endif // INSPECTORMESHFILEWIDGET_H
