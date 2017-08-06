#ifndef UIIMAGERENDERER_H
#define UIIMAGERENDERER_H

#include "Bang/Texture2D.h"
#include "Bang/UIRenderer.h"

class UIImageRenderer : public UIRenderer
{
    COMPONENT(UIImageRenderer)

public:
    UIImageRenderer();
    virtual ~UIImageRenderer();

    virtual void CloneInto(ICloneable *clone) const override;

    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

    void SetImage(Texture2D *imageTexture);

    Texture2D *GetImageTexture() const;

private:
    Texture2D *m_imageTexture = nullptr;
};

#endif // UIIMAGERENDERER_H
