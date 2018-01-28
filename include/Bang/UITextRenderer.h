#ifndef UITEXTRENDERER_H
#define UITEXTRENDERER_H

#include "Bang/Mesh.h"
#include "Bang/Rect.h"
#include "Bang/String.h"
#include "Bang/Vector2.h"
#include "Bang/Alignment.h"
#include "Bang/UIRenderer.h"
#include "Bang/ILayoutElement.h"
#include "Bang/ITransformListener.h"

NAMESPACE_BANG_BEGIN

FORWARD class Font;

class UITextRenderer : public UIRenderer,
                       public ILayoutElement,
                       public IInvalidatable<UITextRenderer>
{
    COMPONENT(UITextRenderer)

public:
    void RegenerateCharQuadsVAO() const;

    void SetFont (Font* font);
    void SetTextColor(const Color &textColor);
    void SetHorizontalAlign(HorizontalAlignment horizontalAlignment);
    void SetVerticalAlign(VerticalAlignment verticalAlignment);
    void SetKerning(bool kerning);
    void SetWrapping(bool wrapping);
    void SetContent(const String &content);
    void SetTextSize(int size);
    void SetSpacingMultiplier(const Vector2 &spacingMultiplier);

    Font* GetFont() const;
    bool IsKerning() const;
    const Color& GetTextColor() const;
    bool IsWrapping() const;
    VerticalAlignment GetVerticalAlignment() const;
    HorizontalAlignment GetHorizontalAlignment() const;
    const String& GetContent() const;
    int GetTextSize() const;
    const Vector2& GetSpacingMultiplier() const;
    const Array<Rect>& GetCharRectsLocalNDC() const;
    const Rect& GetCharRectLocalNDC(uint charIndex) const;
    Rect GetCharRectViewportNDC(uint charIndex) const;
    Rect GetContentViewportNDCRect() const;
    virtual Rect GetBoundingRect(Camera *camera = nullptr) const override;

    // UIRenderer
    virtual void OnRender() override;
    virtual void Bind() const override;
    virtual void UnBind() const override;

    // ILayoutElement
    virtual void CalculateLayout(Axis axis) override;

    // ITransformListener
    virtual void OnTransformChanged() override;

    // ICloneable
    virtual void CloneInto(ICloneable *clone) const override;

    // Serializable
    virtual void ImportXML(const XMLNode &xmlInfo) override;
    virtual void ExportXML(XMLNode *xmlInfo) const override;

private:
    RH<Font> p_font;
    String m_content            = "";
    int m_textSize              = 64;
    Vector2 m_spacingMultiplier = Vector2::One;
    bool m_kerning              = false;
    mutable Rect m_textRectNDC  = Rect::Zero;

    bool m_wrapping = false;
    HorizontalAlignment m_horizontalAlignment = HorizontalAlignment::Center;
    VerticalAlignment m_verticalAlignment     = VerticalAlignment::Center;

    RH<Mesh> p_mesh;
    mutable uint m_numberOfLines = 0;
    mutable Array<Rect> m_charRectsLocalNDC;

    UITextRenderer();
    virtual ~UITextRenderer();

    void OnChanged();
};

NAMESPACE_BANG_END

#endif // UITEXTRENDERER_H
