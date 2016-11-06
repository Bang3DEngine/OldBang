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

    #ifdef BANG_EDITOR
    virtual void OnInspectorXMLNeeded(XMLNode *xmlInfo) const override;
    virtual void OnInspectorXMLChanged(const XMLNode *xmlInfo) override;
    #endif

    virtual void ReadXMLInfo(const XMLNode *xmlInfo) override;
    virtual void FillXMLInfo(XMLNode *xmlInfo) const override;

    virtual void RenderCustomPR() const override;

    void SetContent(const String &content);
    const String& GetContent() const;

    void SetTextSize(int size);
    int GetTextSize() const;

    virtual Rect GetBoundingRect(Camera *camera = nullptr) const override;

private:
    Material *m_letterQuadMaterial = nullptr;

    Font *m_font = nullptr;
    Color m_textColor = Color::Black;
    int m_textSize = 32; // Set in the constructor
    String m_content = "";

    void DeleteCurrentLetterQuads();

    friend class GameObject;
};

#endif // UITEXT_H
