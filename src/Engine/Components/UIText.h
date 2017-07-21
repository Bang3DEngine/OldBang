#ifndef UITEXT_H
#define UITEXT_H

#include "Bang/Mesh.h"
#include "Bang/Rect.h"
#include "Bang/Color.h"
#include "Bang/String.h"
#include "Bang/Vector2.h"
#include "Bang/UIRenderer.h"

class Font;
class XMLNode;
class ICloneable;
class UIText : public UIRenderer
{
    OBJECT(UIText)

public:
    enum class HorizontalAlignment { Left = 0, Center, Right };
    enum class VerticalAlignment   { Top  = 0, Center, Bot   };

    UIText();
    virtual ~UIText();

    virtual void CloneInto(ICloneable *clone) const override;

    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

    void SetHorizontalAlign(HorizontalAlignment horizontalAlignment);
    HorizontalAlignment GetHorizontalAlignment() const;

    void SetVerticalAlign(VerticalAlignment verticalAlignment);
    VerticalAlignment GetVerticalAlignment() const;

    void SetFont (Font *font);
    Font* GetFont() const;

    void SetKerning(bool kerning);
    bool GetKerning() const;

    void SetContent(const String &content);
    const String& GetContent() const;

    void SetTextSize(int size);
    int GetTextSize() const;

    void SetSpacing(const Vector2 &spacing);
    Vector2 GetSpacing() const;

    Rect GetNDCRect() const;

    virtual void Bind() const override;
    virtual void UnBind() const override;

    void OnParentSizeChanged() override;
    void OnDrawGizmos(GizmosPassType gizmosPassType) override;
    virtual Rect GetBoundingRect(Camera *camera = nullptr) const override;

protected:
    String m_content   = "";
    Font *m_font       = nullptr;
    int m_textSize     = 64;
    Vector2 m_spacing  = Vector2(5, 200);
    bool m_kerning     = false;
    Rect m_textRectNDC = Rect::Empty;

    HorizontalAlignment m_horizontalAlignment = HorizontalAlignment::Left;
    VerticalAlignment m_verticalAlignment     = VerticalAlignment::Top;

    Vector2 GetTextSizeScaled() const;
    Rect GetCharRect(char c) const;
    float GetCharAdvance(char current, char next = '\0') const;

    void FillQuadsMeshPositions();
    void FillQuadsMeshUvs();
    void RefreshMesh();

    Vector2 GetAlignmentOffset(const Rect& contentRect) const;
};

#endif // UITEXT_H
