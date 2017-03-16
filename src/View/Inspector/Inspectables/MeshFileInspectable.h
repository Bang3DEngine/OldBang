#ifndef INSPECTORMESHFILEWIDGET_H
#define INSPECTORMESHFILEWIDGET_H

#include "MeshFile.h"
#include "SerializableObject.h"

class XMLNode;
class MeshFileInspectable : public SerializableObject
{
private:
    MeshFile m_fileMesh;

public:
    MeshFileInspectable(const MeshFile &fileMesh);

    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;
};

#endif // INSPECTORMESHFILEWIDGET_H
