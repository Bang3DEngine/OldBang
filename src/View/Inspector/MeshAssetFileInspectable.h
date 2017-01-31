#ifndef MESHASSETFILEINSPECTABLE_H
#define MESHASSETFILEINSPECTABLE_H

#include "IInspectable.h"
#include "MeshAssetFile.h"

class XMLNode;
class MeshAssetFileInspectable : public IInspectable
{
public:
    MeshAssetFileInspectable(const MeshAssetFile &meshFile);

    virtual void OnInspectorXMLChanged(const XMLNode *xmlInfo) override;
    virtual void OnInspectorXMLNeeded(XMLNode *xmlInfo) const override;

private:
    MeshAssetFile m_meshAssetFile;
};

#endif // MESHASSETFILEINSPECTABLE_H
