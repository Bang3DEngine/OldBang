#ifndef INSPECTORMESHFILEWIDGET_H
#define INSPECTORMESHFILEWIDGET_H

#include "Bang/MeshFile.h"
#include "Bang/SerializableObject.h"

class XMLNode;
class MeshFileInspectable : public SerializableObject
{
private:
    MeshFile m_fileMesh;

public:
    MeshFileInspectable(const MeshFile &fileMesh);

    virtual const QPixmap& GetIcon() const override;

    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;
};

#endif // INSPECTORMESHFILEWIDGET_H
