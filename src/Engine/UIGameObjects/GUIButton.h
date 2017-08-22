#ifndef GUIBUTTON_H
#define GUIBUTTON_H

#include "Bang/UIBorderRect.h"
#include "Bang/UIGameObject.h"
#include "Bang/UIButtonTinter.h"

FORWARD class GUILabel;
FORWARD class GUIImage;
FORWARD class UIImageRenderer;

class GUIButton : public UIGameObject
{
    SERIALIZABLE_OBJECT(GUIButton)

public:
    GUIButton(const String &labelText = "Bang");
    virtual ~GUIButton();

    GUILabel *GetLabel() const;
    GUIImage *GetImage() const;

    UIBorderRect   *GetUIBorderRect() const;
    UIButtonTinter *GetLabelTinter() const;
    UIButtonTinter *GetBackgroundTinter() const;

    void AddClickedCallback(UIButton::ClickedCallback callback);

private:
    GUILabel *p_label = nullptr;
    GUIImage *p_bgImage = nullptr;

    UIBorderRect *p_borderRect = nullptr;
    UIButtonTinter *p_bgTinter = nullptr;
    UIButtonTinter *p_labelTinter = nullptr;
};

#endif // GUIBUTTON_H
