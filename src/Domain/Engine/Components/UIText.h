#ifndef UITEXT_H
#define UITEXT_H

#include "UIRenderer.h"

class Font;
class UIText : public UIRenderer
{

public:
    UIText();
    virtual ~UIText();

    virtual void Render() const override;

    virtual const String ToString() const override;
    virtual String GetName() const override;
    virtual void CloneInto(ICloneable *clone) const override;
    virtual ICloneable *Clone() const override;

    virtual void ReadXMLInfo(const XMLNode *xmlInfo) override;
    virtual void FillXMLInfo(XMLNode *xmlInfo) const override;

    virtual void RenderCustomPR() const override;

private:
    Font *m_font = nullptr;
    Color m_textColor = Color::Black;
    String m_content = "";

    friend class GameObject;
};

#endif // UITEXT_H
