#ifndef UITEXT_H
#define UITEXT_H

#include "Bang/Mesh.h"
#include "Bang/Rect.h"
#include "Bang/Color.h"
#include "Bang/String.h"
#include "Bang/UIRenderer.h"

class Font;
class XMLNode;
class ICloneable;
class UIText : public UIRenderer
{
    OBJECT(UIText)
    ICLONEABLE(UIText)
    COMPONENT_ICON(UIText, "Icons/LetterIcon.png")

public:
    enum class HorizontalAlignment { Left = 0, Center, Right };
    enum class VerticalAlignment   { Top  = 0, Center, Bot   };

    UIText();
    virtual ~UIText();

    virtual void RenderWithoutMaterial() const override;
    virtual void RenderForSelectionWithoutMaterial() const override;

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

    void SetHorizontalSpacing(int horizontalSpacing);
    int GetHorizontalSpacing() const;

    void GetContentNDCBounds(Vector2 *min, Vector2 *max,
                             bool applyAlign = true) const;

    virtual Rect GetBoundingRect(Camera *camera = nullptr) const override;

private:
    String m_content = "";

    mutable Mesh m_textQuadsMesh; // Positions and uvs in atlas of each char quad

    Font *m_font = nullptr;
    int m_textSize = 32; // Set in the constructor
    int m_horizontalSpacing = 5;
    bool m_kerning = false;

    HorizontalAlignment m_horizontalAlignment = HorizontalAlignment::Left;
    VerticalAlignment m_verticalAlignment     = VerticalAlignment::Center;

    bool IsValidChar(char c) const;

    Vector2 GetTextSizeNDC() const;
    Rect GetNDCRectOfChar(char c) const;
    Rect GetNDCRectOfChar(char c, const Rect &screenRectNDC) const;
    float GetNDCAdvance(char current, char next = '\0') const;

    void FillQuadsMeshPositions();
    void FillQuadsMeshUvs();

    Vector2 GetAlignmentNDCOffset() const;
};

#endif // UITEXT_H
