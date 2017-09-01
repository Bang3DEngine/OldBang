
#ifndef UITEXTRENDERER_H
#define UITEXTRENDERER_H

#include "Bang/Mesh.h"
#include "Bang/Rect.h"
#include "Bang/String.h"
#include "Bang/Vector2.h"
#include "Bang/TextFormat.h"
#include "Bang/UIRenderer.h"

FORWARD class Font;

class UITextRenderer : public UIRenderer
{
    COMPONENT(UITextRenderer)

public:
    UITextRenderer();
    virtual ~UITextRenderer();

    // UIRenderer
    virtual void OnRender() override;
    virtual void OnUpdate() override;
    virtual void OnParentSizeChanged() override;

    // UIRenderer
    virtual void Bind() const override;
    virtual void UnBind() const override;

    void RefreshMesh();

    void SetFont (Font *font);
    void SetTextColor(const Color &textColor);
    void SetHorizontalAlign(HorizontalAlignment horizontalAlignment);
    void SetVerticalAlign(VerticalAlignment verticalAlignment);
    void SetKerning(bool kerning);
    void SetWrapping(bool wrapping);
    void SetAlphaThreshold(float alphaThreshold);
    void SetBlurriness(float blurriness);
    void SetContent(const String &content);
    void SetTextSize(int size);
    void SetExtraSpacing(const Vector2i &extraSpacing);

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
    Vector2i GetExtraSpacing() const;
    const Array<Rect>& GetCharRectsLocalNDC() const;
    const Rect& GetCharRectLocalNDC(uint charIndex) const;
    Rect GetContentGlobalNDCRect() const;
    virtual Rect GetBoundingRect(Camera *camera = nullptr) const override;

    // ICloneable
    virtual void CloneInto(ICloneable *clone) const override;

    // Serializable
    virtual void ImportXML(const XMLNode &xmlInfo) override;
    virtual void ExportXML(XMLNode *xmlInfo) const override;

protected:
    String m_content        = "";
    Font *m_font            = nullptr;
    int m_textSize          = 64;
    Vector2i m_extraSpacing = Vector2i::Zero;
    bool m_kerning          = false;
    Rect m_textRectNDC      = Rect::Zero;

    float m_blurriness      = 0.1f;
    float m_alphaThreshold  = 0.7f;

    bool m_hasChanged       = false;

    bool m_wrapping = false;
    HorizontalAlignment m_horizontalAlignment = HorizontalAlignment::Left;
    VerticalAlignment m_verticalAlignment     = VerticalAlignment::Top;

    Array<Rect> m_charRectsLocalNDC;

private:
    Mesh *m_mesh = nullptr;
};

#endif // UITEXTRENDERER_H
