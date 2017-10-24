#ifndef UITEXTRENDERER_H
#define UITEXTRENDERER_H

#include "Bang/Mesh.h"
#include "Bang/Rect.h"
#include "Bang/String.h"
#include "Bang/Vector2.h"
#include "Bang/Alignment.h"
#include "Bang/UIRenderer.h"
#include "Bang/ILayoutElement.h"
#include "Bang/IRectTransformListener.h"

NAMESPACE_BANG_BEGIN

FORWARD class Font;

class UITextRenderer : public UIRenderer,
                       public ILayoutElement,
                       public IRectTransformListener,
                       public IInvalidatable<UITextRenderer>
{
    COMPONENT(UITextRenderer)
    IINVALIDATABLE(UITextRenderer)

public:
    enum class LayoutMode
    {
        SingleLineMinPreferred,
        MultiLineMinPreferred,
        SingleLinePreferred,
        MultiLinePreferred
    };

    virtual ~UITextRenderer();

    // UIRenderer
    virtual void OnRender() override;
    virtual void Bind() const override;
    virtual void UnBind() const override;
    virtual void OnRender(RenderPass renderPass) override;

    // ILayoutElement
    virtual Vector2i _GetMinSize()       const override;
    virtual Vector2i _GetPreferredSize() const override;
    virtual Vector2  _GetFlexibleSize()  const override;

    void RegenerateCharQuadsVAO() const;

    void SetFont (Font *font);
    void SetTextColor(const Color &textColor);
    void SetHorizontalAlign(HorizontalAlignment horizontalAlignment);
    void SetVerticalAlign(VerticalAlignment verticalAlignment);
    void SetKerning(bool kerning);
    void SetWrapping(bool wrapping);
    void SetOverlapping(bool overlapping);
    void SetAlphaThreshold(float alphaThreshold);
    void SetBlurriness(float blurriness);
    void SetContent(const String &content);
    void SetTextSize(int size);
    void SetOutlineWidth(float outlineWidth);
    void SetOutlineColor(const Color &color);
    void SetOutlineBlurriness(float outlineBlurriness);
    void SetSpacingMultiplier(const Vector2 &spacingMultiplier);
    void SetLayoutMode(LayoutMode layoutMode);

    virtual bool NeedsReadingColorBuffer() const override;
    Font* GetFont() const;
    bool IsKerning() const;
    const Color& GetTextColor() const;
    bool IsWrapping() const;
    float GetBlurriness() const;
    float GetAlphaThreshold() const;
    VerticalAlignment GetVerticalAlignment() const;
    HorizontalAlignment GetHorizontalAlignment() const;
    const String& GetContent() const;
    int GetTextSize() const;
    bool IsOverlapping() const;
    float GetOutlineWidth() const;
    const Color& GetOutlineColor() const;
    float GetOutlineBlurriness() const;
    const Vector2& GetSpacingMultiplier() const;
    const Array<Rect>& GetCharRectsLocalNDC() const;
    const Rect& GetCharRectLocalNDC(uint charIndex) const;
    Rect GetContentGlobalNDCRect() const;
    LayoutMode GetLayoutMode() const;
    virtual Rect GetBoundingRect(Camera *camera = nullptr) const override;

    // IRectTransformListener
    virtual void OnRectTransformChanged() override;

    // ICloneable
    virtual void CloneInto(ICloneable *clone) const override;

    // Serializable
    virtual void ImportXML(const XMLNode &xmlInfo) override;
    virtual void ExportXML(XMLNode *xmlInfo) const override;

private:
    String m_content            = "";
    Font *m_font                = nullptr;
    int m_textSize              = 64;
    Vector2 m_spacingMultiplier = Vector2::One;
    bool m_kerning              = false;
    mutable Rect m_textRectNDC  = Rect::Zero;

    float m_blurriness      = 1.2f;
    float m_alphaThreshold  = 0.24f;

    float m_outlineWidth        = 0.0f;
    Color m_outlineColor        = Color::Red;
    float m_outlineBlurriness   = 0.01f;

    bool m_isOverlapping = false;

    bool m_wrapping = false;
    HorizontalAlignment m_horizontalAlignment = HorizontalAlignment::Center;
    VerticalAlignment m_verticalAlignment     = VerticalAlignment::Center;

    Mesh *m_mesh = nullptr;
    uint m_currentRenderingChar = 0;
    mutable Array<Rect> m_charRectsLocalNDC;

    LayoutMode m_layoutMode = LayoutMode::SingleLinePreferred;

    UITextRenderer();

    void OnChanged();

    friend class ComponentFactory;
};

NAMESPACE_BANG_END

#endif // UITEXTRENDERER_H
