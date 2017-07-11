#ifndef UIIMAGE_H
#define UIIMAGE_H

#include "Bang/String.h"
#include "Bang/Texture2D.h"
#include "Bang/UIRenderer.h"

class XMLNode;
class ICloneable;
class UIImage : public UIRenderer
{
    OBJECT(UIImage)

public:
    UIImage();
    virtual ~UIImage();

    virtual void CloneInto(ICloneable *clone) const override;

    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

    void SetImage(Texture2D *imageTexture);

    Texture2D *GetImageTexture() const;

private:
    Texture2D *m_imageTexture = nullptr;
};

#endif // UIIMAGE_H