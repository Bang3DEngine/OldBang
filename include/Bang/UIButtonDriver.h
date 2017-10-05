#ifndef UIBUTTONDRIVER_H
#define UIBUTTONDRIVER_H

#include "Bang/Component.h"
#include "Bang/IComponentDriver.h"

NAMESPACE_BANG_BEGIN

FORWARD class UIBorderRect;
FORWARD class UITextRenderer;
FORWARD class UIImageRenderer;
FORWARD class UITintedButton;

class UIButtonDriver : public Component,
                       public IComponentDriver<UIButtonDriver>
{
    COMPONENT(UIButtonDriver)

public:
    virtual ~UIButtonDriver();

    UIBorderRect* GetBorder() const;
    UITextRenderer* GetText() const;
    UIImageRenderer* GetBackground() const;
    UITintedButton* GetButton() const;

private:
    UIButtonDriver();

    UIBorderRect    *p_border      = nullptr;
    UITextRenderer  *p_text        = nullptr;
    UIImageRenderer *p_background  = nullptr;
    UITintedButton  *p_button      = nullptr;

    void SetBorder(UIBorderRect *borderRect);
    void SetText(UITextRenderer *textRenderer);
    void SetBackground(UIImageRenderer *imgRenderer);
    void SetButton(UITintedButton *button);

    // IComponentDriver
    static UIButtonDriver *CreateInto(GameObject *go);

    friend class GameObjectFactory;
    friend class IComponentDriver<UIButtonDriver>;
};

NAMESPACE_BANG_END

#endif // UIBUTTONDRIVER_H
