#ifndef GUIBUTTON_H
#define GUIBUTTON_H

#include "Bang/UIGameObject.h"
#include "Bang/UIButtonTinter.h"

FORWARD class GUILabel;
FORWARD class GUIImage;
FORWARD class UIImageRenderer;

class GUIButton : public UIGameObject
{
public:
    GUIButton(const String &labelText = "Bang");
    virtual ~GUIButton();

    GUILabel *GetLabel() const;
    GUIImage *GetImage() const;

    UIButtonTinter *GetLabelTinter() const;
    UIButtonTinter *GetBackgroundTinter() const;

private:
    GUILabel *p_label = nullptr;
    GUIImage *p_bgImage = nullptr;

    UIButtonTinter *p_bgTinter = nullptr;
    UIButtonTinter *p_labelTinter = nullptr;
};

#endif // GUIBUTTON_H
