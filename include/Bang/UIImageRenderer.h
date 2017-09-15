#ifndef UIIMAGERENDERER_H
#define UIIMAGERENDERER_H

#include "Bang/Texture2D.h"
#include "Bang/Alignment.h"
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
    void SetIsBackground(bool isBackground);
    void SetAspectRatioMode(AspectRatioMode arMode);
    void SetVerticalAlignment(VerticalAlignment verticalAlignment);
    void SetHorizontalAlignment(HorizontalAlignment horizontalAlignment);

    const Color& GetTint() const;
    Texture2D *GetImageTexture() const;
    AspectRatioMode GetAspectRatioMode() const;
    VerticalAlignment GetVerticalAlignment() const;
    HorizontalAlignment GetHorizontalAlignment() const;
    bool IsBackground() const;

    // Renderer
    virtual Rect GetBoundingRect(Camera *camera = nullptr) const override;

    // ICloneable
    virtual void CloneInto(ICloneable *clone) const override;

    // Serializable
    virtual void ImportXML(const XMLNode &xmlInfo) override;
    virtual void ExportXML(XMLNode *xmlInfo) const override;

private:
    Mesh *m_quadMesh = nullptr;
    bool m_isBackground = false;
    Texture2D *m_imageTexture = nullptr;

    AspectRatioMode m_aspectRatioMode         = AspectRatioMode::Ignore;
    VerticalAlignment m_verticalAlignment     = VerticalAlignment::Center;
    HorizontalAlignment m_horizontalAlignment = HorizontalAlignment::Center;

    Vector2i m_prevRectSize = Vector2i::Zero;
    Vector2i m_prevImageTextureSize = Vector2i::Zero;

    void UpdateQuadUvsToMatchFormat(bool force = false);
};

NAMESPACE_BANG_END

#endif // UIIMAGERENDERER_H
