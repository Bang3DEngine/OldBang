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

    // UIRenderer
    virtual void OnRender() override;

    void SetImage(Texture2D *imageTexture);
    void SetTint(const Color& tint);

    const Color& GetTint() const;
    Texture2D *GetImageTexture() const;

    // ICloneable
    virtual void CloneInto(ICloneable *clone) const override;

    // SerializableObject
    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

private:
    Mesh *p_quadMesh = nullptr;
    Texture2D *m_imageTexture = nullptr;
};

#endif // UIIMAGERENDERER_H