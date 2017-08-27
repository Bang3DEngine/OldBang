#ifndef GUIBUTTON_H
#define GUIBUTTON_H

#include "Bang/UIComponent.h"
#include "Bang/UIBorderRect.h"
#include "Bang/UIButtonTinter.h"

FORWARD class GUILabel;
FORWARD class GUIImage;
FORWARD class UIImageRenderer;

class GUIButton : public UIComponent
{
    UICOMPONENT(GUIButton)

public:
    GUIButton();
    virtual ~GUIButton();

    GUILabel *GetLabel() const;
    GUIImage *GetImage() const;

    void AddClickedCallback(UIButton::ClickedCallback callback);

private:
    GUILabel *p_label = nullptr;
    GUIImage *p_bgImage = nullptr;

    UIBorderRect *p_borderRect = nullptr;
    UIButtonTinter *p_bgTinter = nullptr;
    UIButtonTinter *p_labelTinter = nullptr;

    static UIGameObject *CreateGameObject();
    void RetrieveReferences();
    void InitGameObject();

    friend class GameObjectFactory;
};

#endif // GUIBUTTON_H
