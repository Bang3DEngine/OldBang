#ifndef GUILABEL_H
#define GUILABEL_H

#include "Bang/UIGameObject.h"

FORWARD class UIText;

class GUILabel : public UIGameObject
{
public:
    GUILabel(const String &content = "");

    UIText *GetText();
    const UIText *GetText() const;

private:
    UIText *p_text = nullptr;
};

#endif // GUILABEL_H
