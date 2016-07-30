#ifndef MESHASSETFILEINSPECTABLE_H
#define MESHASSETFILEINSPECTABLE_H

#include "XMLNode.h"
#include "IInspectable.h"
#include "MeshAssetFile.h"

class MeshAssetFileInspectable : public IInspectable
{
private:
    MeshAssetFile m_meshFile;

public:
    MeshAssetFileInspectable(const MeshAssetFile &meshFile);

    virtual void OnInspectorXMLChanged(const XMLNode *xmlInfo) override;
    virtual void OnInspectorXMLNeeded(XMLNode *xmlInfo) const override;
};

#endif // MESHASSETFILEINSPECTABLE_H
