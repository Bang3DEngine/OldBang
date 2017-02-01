#ifndef MATERIALASSETFILEINSPECTABLE_H
#define MATERIALASSETFILEINSPECTABLE_H

#include "XMLNode.h"
#include "IInspectable.h"
#include "MaterialAssetFile.h"

class MaterialAssetFileInspectable : public IInspectable
{
private:
    XMLNode m_xmlInfo;
    MaterialAssetFile m_materialAssetFile;

public:
    MaterialAssetFileInspectable(const MaterialAssetFile &materialAssetFile);

    virtual void OnInspectorXMLChanged(const XMLNode *xmlInfo) override;
    virtual void OnInspectorXMLNeeded(XMLNode *xmlInfo) const override;
};

#endif // MATERIALASSETFILEINSPECTABLE_H
