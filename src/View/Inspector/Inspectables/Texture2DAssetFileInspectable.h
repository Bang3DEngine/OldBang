#ifndef INSPECTORTEXTURE2DASSETWIDGET_H
#define INSPECTORTEXTURE2DASSETWIDGET_H

#include "Bang/XMLNode.h"
#include "Bang/SerializableObject.h"
#include "Bang/Texture2DAssetFile.h"

class Texture2DAssetFileInspectable : public SerializableObject
{
    OBJECT(Texture2DAssetFileInspectable)

private:
    XMLNode m_xmlInfo;
    Texture2DAssetFile m_fileTex;

public:
    Texture2DAssetFileInspectable();
    Texture2DAssetFileInspectable(const Texture2DAssetFile &fileTex);

    virtual const QPixmap& GetIcon() const override;

    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;
};

#endif // INSPECTORTEXTURE2DASSETWIDGET_H
