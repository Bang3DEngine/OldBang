#ifndef UITEXT_H
#define UITEXT_H

#include "Rect.h"
#include "Color.h"
#include "String.h"
#include "UIRenderer.h"

class Font;
class XMLNode;
class ICloneable;
class UIText : public UIRenderer
{
public:
    enum class HorizontalAlignment { Left = 0, Center, Right };
    enum class VerticalAlignment { Top = 0, Center, Bot };

    UIText();
    virtual ~UIText();

    virtual void Render() const override;
    virtual void RenderForSelectionFramebufferWithoutBindingMaterial() const
                override;

    virtual String GetName() const override;

    virtual void CloneInto(ICloneable *clone) const override;
    virtual ICloneable *Clone() const override;

    virtual void ReadXMLInfo(const XMLNode *xmlInfo) override;
    virtual void FillXMLInfo(XMLNode *xmlInfo) const override;

    virtual void RenderCustomSP() const override;

    void SetHorizontalAlign(HorizontalAlignment horizontalAlignment);
    HorizontalAlignment GetHorizontalAlignment() const;

    void SetVerticalAlign(VerticalAlignment verticalAlignment);
    VerticalAlignment GetVerticalAlignment() const;

    void SetColor (const Color &color);
    Color GetColor() const;

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
    Font *m_font = nullptr;
    Color m_textColor = Color::Black;
    int m_textSize = 32; // Set in the constructor
    int m_horizontalSpacing = 5;
    bool m_kerning = false;

    HorizontalAlignment m_horizontalAlignment = HorizontalAlignment::Left;
    VerticalAlignment m_verticalAlignment = VerticalAlignment::Center;

    String m_content = "";

    void RenderText() const;

    bool IsValidChar(char c) const;

    Vector2 GetTextSizeNDC() const;
    Rect GetNDCRectOfChar(char c) const;
    Rect GetNDCRectOfChar(char c, const Rect &screenRectNDC) const;
    float GetNDCAdvance(char current, char next = '\0') const;

    Vector2 GetAlignmentNDCOffset() const;

    friend class GameObject;
};

#endif // UITEXT_H
