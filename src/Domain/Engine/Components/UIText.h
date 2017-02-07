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
    UIText();
    virtual ~UIText();

    virtual void Render() const override;
    virtual void RenderForSelectionFramebufferWithoutBindingMaterial() const override;

    virtual String GetName() const override;

    virtual void CloneInto(ICloneable *clone) const override;
    virtual ICloneable *Clone() const override;

    virtual void ReadXMLInfo(const XMLNode *xmlInfo) override;
    virtual void FillXMLInfo(XMLNode *xmlInfo) const override;

    virtual void RenderCustomPR() const override;

    void SetContent(const String &content);
    const String& GetContent() const;

    void SetTextSize(int size);
    int GetTextSize() const;

    virtual Rect GetBoundingRect(Camera *camera = nullptr) const override;

private:
    Font *m_font = nullptr;
    Color m_textColor = Color::Black;
    int m_textSize = 32; // Set in the constructor
    String m_content = "";

    void RenderText(bool forSelectionFramebuffer) const;
    friend class GameObject;
};

#endif // UITEXT_H
