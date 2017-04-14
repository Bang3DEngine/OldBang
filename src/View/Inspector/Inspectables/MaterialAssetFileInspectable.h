#ifndef MATERIALASSETFILEINSPECTABLE_H
#define MATERIALASSETFILEINSPECTABLE_H

#include "Bang/XMLNode.h"
#include "Bang/SerializableObject.h"
#include "Bang/MaterialAssetFile.h"

class MaterialAssetFileInspectable : public SerializableObject
{
    OBJECT(MaterialAssetFileInspectable)

private:
    XMLNode m_xmlInfo;
    MaterialAssetFile m_materialAssetFile;

public:
    MaterialAssetFileInspectable();
    MaterialAssetFileInspectable(const MaterialAssetFile &materialAssetFile);

    virtual const QPixmap& GetIcon() const override;

    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;
};

#endif // MATERIALASSETFILEINSPECTABLE_H
