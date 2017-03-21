#ifndef MATERIALASSETFILEINSPECTABLE_H
#define MATERIALASSETFILEINSPECTABLE_H

#include "XMLNode.h"
#include "SerializableObject.h"
#include "MaterialAssetFile.h"

class MaterialAssetFileInspectable : public SerializableObject
{
private:
    XMLNode m_xmlInfo;
    MaterialAssetFile m_materialAssetFile;

public:
    MaterialAssetFileInspectable(const MaterialAssetFile &materialAssetFile);

    virtual const QPixmap& GetIcon() const override;

    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;
};

#endif // MATERIALASSETFILEINSPECTABLE_H
