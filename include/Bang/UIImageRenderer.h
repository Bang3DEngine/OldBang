#ifndef UIIMAGERENDERER_H
#define UIIMAGERENDERER_H

#include "Bang/Texture2D.h"
#include "Bang/UIRenderer.h"
#include "Bang/AspectRatioMode.h"

NAMESPACE_BANG_BEGIN

class UIImageRenderer : public UIRenderer
{
    COMPONENT(UIImageRenderer)

public:
    UIImageRenderer();
    virtual ~UIImageRenderer();

    // UIRenderer
    virtual void OnUpdate() override;
    virtual void OnRender() override;

    void SetImageTexture(const Path &imagePath);
    void SetImageTexture(Texture2D *imageTexture);
    void SetTint(const Color& tint);
    void SetAspectRatioMode(AspectRatioMode arMode);

    const Color& GetTint() const;
    Texture2D *GetImageTexture() const;
    AspectRatioMode GetAspectRatioMode() const;

    // ICloneable
    virtual void CloneInto(ICloneable *clone) const override;

    // Serializable
    virtual void ImportXML(const XMLNode &xmlInfo) override;
    virtual void ExportXML(XMLNode *xmlInfo) const override;

private:
    Mesh *p_quadMesh = nullptr;
    Texture2D *m_imageTexture = nullptr;
    AspectRatioMode m_aspectRatioMode = AspectRatioMode::Ignore;

    Vector2i m_prevRectSize = Vector2i::Zero;
    Vector2i m_prevImageTextureSize = Vector2i::Zero;

    void UpdateQuadUvsToMatchAspectRatioMode();
};

NAMESPACE_BANG_END

#endif // UIIMAGERENDERER_H
