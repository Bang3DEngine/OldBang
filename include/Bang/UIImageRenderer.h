#ifndef UIIMAGERENDERER_H
#define UIIMAGERENDERER_H

#include "Bang/Texture2D.h"
#include "Bang/Alignment.h"
#include "Bang/UIRenderer.h"
#include "Bang/AspectRatioMode.h"
#include "Bang/ITransformListener.h"

NAMESPACE_BANG_BEGIN

class UIImageRenderer : public UIRenderer
{
    COMPONENT(UIImageRenderer)

public:
    UIImageRenderer();
    virtual ~UIImageRenderer();

    // UIRenderer
    virtual void OnRender() override;

    void SetImageTexture(const Path &imagePath);
    void SetImageTexture(Texture2D* imageTexture);
    void SetTint(const Color& tint);

    const Color& GetTint() const;
    Texture2D *GetImageTexture() const;

    // ITransformListener
    virtual void OnTransformChanged() override;

    // Renderer
    virtual Rect GetBoundingRect(Camera *camera = nullptr) const override;

    // ICloneable
    virtual void CloneInto(ICloneable *clone) const override;

    // Serializable
    virtual void ImportXML(const XMLNode &xmlInfo) override;
    virtual void ExportXML(XMLNode *xmlInfo) const override;

private:
    RH<Mesh> p_quadMesh;
    RH<Texture2D> p_imageTexture;
};

NAMESPACE_BANG_END

#endif // UIIMAGERENDERER_H
