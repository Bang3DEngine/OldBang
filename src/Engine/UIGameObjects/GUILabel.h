#ifndef GUILABEL_H
#define GUILABEL_H

#include "Bang/UIGameObject.h"
#include "Bang/UITextRenderer.h"

FORWARD class GUIMask;
FORWARD class UITextRenderer;

class GUILabel : public UIGameObject
{
public:
    GUILabel(const String &content = "Bang");

    GUIMask *GetMask();
    UITextRenderer *GetText();
    const UITextRenderer *GetText() const;

private:
    UITextRenderer *p_text = nullptr;
    GUIMask *m_mask = nullptr;
    UIGameObject *m_textContainer = nullptr;
};

#endif // GUILABEL_H
