#ifndef UIBUTTONDRIVER_H
#define UIBUTTONDRIVER_H

#include "Bang/Component.h"
#include "Bang/UIButtoneable.h"

NAMESPACE_BANG_BEGIN

FORWARD class Texture2D;
FORWARD class UIDirLayout;
FORWARD class UITextRenderer;
FORWARD class UIImageRenderer;

class UIButton : public Component,
                 public IUIButtonListener
{
    COMPONENT(UIButton)

public:
    void SetIconSize(const Vector2i &size);
    void SetIconTexture(Texture2D *texture);
    void SetIconSpacingWithText(int spacingWithText);
    void SetIcon(Texture2D *texture, const Vector2i &size,
                 int spacingWithText = 5);

    UIImageRenderer* GetIcon() const;
    UITextRenderer* GetText() const;
    UIImageRenderer* GetBackground() const;
    UIDirLayout *GetDirLayout() const;
    UIButtoneable* GetButton() const;

private:
    UIButton();
    virtual ~UIButton();

    UIImageRenderer *p_icon        = nullptr;
    UITextRenderer  *p_text        = nullptr;
    UIImageRenderer *p_background  = nullptr;
    UIButtoneable        *p_button      = nullptr;

    static UIButton *CreateInto(GameObject *go);

    // IUIButtonListener
    virtual void OnButton_MouseEnter(UIButtoneable *btn) override;
    virtual void OnButton_MouseExit(UIButtoneable *btn) override;
    virtual void OnButton_MouseDown(UIButtoneable *btn, MouseButton mb) override;
    virtual void OnButton_MouseUp(UIButtoneable *btn, MouseButton mb,
                                  bool inside) override;

    friend class GameObjectFactory;
};

NAMESPACE_BANG_END

#endif // UIBUTTONDRIVER_H
