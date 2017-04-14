#ifndef MESHASSETFILEINSPECTABLE_H
#define MESHASSETFILEINSPECTABLE_H

#include "Bang/SerializableObject.h"
#include "Bang/MeshAssetFile.h"

class XMLNode;
class MeshAssetFileInspectable : public SerializableObject
{
    OBJECT(MeshAssetFileInspectable)

public:
    MeshAssetFileInspectable();
    MeshAssetFileInspectable(const MeshAssetFile &meshFile);

    const QPixmap& GetIcon() const override;

    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

private:
    MeshAssetFile m_meshAssetFile;
};

#endif // MESHASSETFILEINSPECTABLE_H
