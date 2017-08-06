#ifndef GUILABEL_H
#define GUILABEL_H

#include "Bang/UIGameObject.h"

FORWARD class UITextRenderer;

class GUILabel : public UIGameObject
{
public:
    GUILabel(const String &content = "");

    UITextRenderer *GetText();
    const UITextRenderer *GetText() const;

private:
    UITextRenderer *p_text = nullptr;
};

#endif // GUILABEL_H
