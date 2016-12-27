#ifndef INSPECTORTEXTURE2DASSETWIDGET_H
#define INSPECTORTEXTURE2DASSETWIDGET_H

#include "XMLNode.h"
#include "IInspectable.h"
#include "Texture2DAssetFile.h"

class Texture2DAssetFileInspectable : public IInspectable
{
private:
    XMLNode m_xmlInfo;
    Texture2DAssetFile m_fileTex;

public:
    Texture2DAssetFileInspectable(const Texture2DAssetFile &fileTex);

    virtual void OnInspectorXMLChanged(const XMLNode *xmlInfo) override;
    virtual void OnInspectorXMLNeeded(XMLNode *xmlInfo) const override;
};

#endif // INSPECTORTEXTURE2DASSETWIDGET_H
