#ifndef GUILABEL_H
#define GUILABEL_H

#include "Bang/UIGameObject.h"
#include "Bang/UITextRenderer.h"

FORWARD class GUIScrollArea;
FORWARD class UITextRenderer;

class GUILabel : public UIGameObject
{
public:
    GUILabel(const String &content = "Bang");

    GUIScrollArea *GetScrollArea();
    UITextRenderer *GetText();
    const UITextRenderer *GetText() const;

private:
    UITextRenderer *p_text = nullptr;
    GUIScrollArea *m_scrollArea = nullptr;
    UIGameObject *m_textContainer = nullptr;
};

#endif // GUILABEL_H
