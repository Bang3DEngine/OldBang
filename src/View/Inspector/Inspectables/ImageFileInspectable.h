#ifndef INSPECTORTEXTURE2DWIDGET_H
#define INSPECTORTEXTURE2DWIDGET_H

#include "ImageFile.h"
#include "SerializableObject.h"

class ImageFileInspectable : public SerializableObject
{
private:
    ImageFile m_fileImage;

public:
    ImageFileInspectable(const ImageFile &fileImage);

    virtual const QPixmap& GetIcon() const override;

    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;
};

#endif // INSPECTORTEXTURE2DWIDGET_H
