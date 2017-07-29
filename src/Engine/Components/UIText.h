#ifndef UITEXT_H
#define UITEXT_H

#include "Bang/Mesh.h"
#include "Bang/Rect.h"
#include "Bang/String.h"
#include "Bang/Vector2.h"
#include "Bang/TextFormat.h"
#include "Bang/UIRenderer.h"

FORWARD class Font;

class UIText : public UIRenderer
{
    OBJECT(UIText)

public:
    UIText();
    virtual ~UIText();

    virtual void CloneInto(ICloneable *clone) const override;

    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

    void SetFont (Font *font);
    void SetHorizontalAlign(HorizontalAlignment horizontalAlignment);
    void SetVerticalAlign(VerticalAlignment verticalAlignment);
    void SetKerning(bool kerning);
    void SetHorizontalWrapMode(WrapMode wrapMode);
    void SetVerticalWrapMode(WrapMode wrapMode);
    void SetContent(const String &content);
    void SetTextSize(int size);

    Font* GetFont() const;
    bool GetKerning() const;
    WrapMode GetHorizontalWrapMode() const;
    WrapMode GetVerticalWrapMode() const;
    VerticalAlignment GetVerticalAlignment() const;
    HorizontalAlignment GetHorizontalAlignment() const;
    void SetSpacing(const Vector2i &spacing);
    const String& GetContent() const;
    int GetTextSize() const;
    Vector2i GetSpacing() const;

    const Rect& GetCharRectNDC(uint charIndex) const;

    Rect GetNDCRect() const;

    virtual void Bind() const override;
    virtual void UnBind() const override;

    void OnParentSizeChanged() override;
    virtual Rect GetBoundingRect(Camera *camera = nullptr) const override;

protected:
    String m_content    = "";
    Font *m_font        = nullptr;
    int m_textSize      = 64;
    Vector2i m_spacing  = Vector2i(5, 200);
    bool m_kerning      = false;
    Rect m_textRectNDC  = Rect::Zero;

    WrapMode m_hWrapMode = WrapMode::Wrap;
    WrapMode m_vWrapMode = WrapMode::Hide;
    HorizontalAlignment m_horizontalAlignment = HorizontalAlignment::Left;
    VerticalAlignment m_verticalAlignment     = VerticalAlignment::Top;

    Array<Rect> m_charRectsNDC;

    void RefreshMesh();
};

#endif // UITEXT_H
