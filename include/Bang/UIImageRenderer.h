#ifndef UIIMAGERENDERER_H
#define UIIMAGERENDERER_H

#include "Bang/Texture2D.h"
#include "Bang/Alignment.h"
#include "Bang/UIRenderer.h"
#include "Bang/ILayoutElement.h"
#include "Bang/AspectRatioMode.h"
#include "Bang/IRectTransformListener.h"

NAMESPACE_BANG_BEGIN

class UIImageRenderer : public UIRenderer,
                        public ILayoutElement,
                        public IRectTransformListener
{
    COMPONENT(UIImageRenderer)

public:
    UIImageRenderer();
    virtual ~UIImageRenderer();

    // UIRenderer
    virtual void OnRender() override;

    // ILayoutElement
    virtual Vector2i CalculateTotalMinSize() const override;
    virtual Vector2i CalculateTotalPreferredSize() const override;
    virtual Vector2i CalculateTotalFlexiblePxSize() const override;

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

    // IRectTransformListener
    virtual void OnRectTransformChanged() override;

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
};

NAMESPACE_BANG_END

#endif // UIIMAGERENDERER_H
