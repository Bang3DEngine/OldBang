#ifndef UIBUTTON_H
#define UIBUTTON_H

#include "Bang/Component.h"

NAMESPACE_BANG_BEGIN

FORWARD class UIBorderRect;
FORWARD class UITextRenderer;
FORWARD class UIImageRenderer;
FORWARD class UITintedInteractive;

class UIButton : public Component
{
    COMPONENT(UIButton)

public:
    UIButton();
    virtual ~UIButton();

    UIBorderRect* GetBorder() const;
    UITextRenderer* GetText() const;
    UIImageRenderer* GetBackground() const;
    UITintedInteractive* GetInteractive() const;

private:
    UIBorderRect        *p_border      = nullptr;
    UITextRenderer      *p_text        = nullptr;
    UIImageRenderer     *p_background  = nullptr;
    UITintedInteractive *p_interactive = nullptr;

    void SetBorder(UIBorderRect *borderRect);
    void SetText(UITextRenderer *textRenderer);
    void SetBackground(UIImageRenderer *imgRenderer);
    void SetInteractive(UITintedInteractive *interactive);

    friend class GameObjectFactory;
};

NAMESPACE_BANG_END

#endif // UIBUTTON_H
