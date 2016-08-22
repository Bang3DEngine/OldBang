#ifndef INSPECTORTEXTURE2DWIDGET_H
#define INSPECTORTEXTURE2DWIDGET_H



#include "ImageFile.h"
#include "InspectorWidget.h"

class ImageFileInspectable : public IInspectable
{
private:
    ImageFile m_fileImage;

public:
    ImageFileInspectable(const ImageFile &fileImage);

    virtual void OnInspectorXMLChanged(const XMLNode *xmlInfo) override;
    virtual void OnInspectorXMLNeeded(XMLNode *xmlInfo) const override;
};

#endif // INSPECTORTEXTURE2DWIDGET_H
