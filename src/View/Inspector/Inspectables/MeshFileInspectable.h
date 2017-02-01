#ifndef INSPECTORMESHFILEWIDGET_H
#define INSPECTORMESHFILEWIDGET_H

#include "MeshFile.h"
#include "IInspectable.h"

class XMLNode;
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
