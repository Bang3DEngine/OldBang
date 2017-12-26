#ifndef UIBUTTONDRIVER_H
#define UIBUTTONDRIVER_H

#include "Bang/Component.h"
#include "Bang/UIFocusable.h"
#include "Bang/IFocusListener.h"

NAMESPACE_BANG_BEGIN

FORWARD class Texture2D;
FORWARD class UIDirLayout;
FORWARD class UITextRenderer;
FORWARD class UIImageRenderer;

class UIButton : public Component,
                 public IFocusListener
{
    COMPONENT(UIButton)

public:
    // Component
    void OnStart() override;
    void OnUpdate() override;

    void Click(bool doubleClick);
    void SetBlocked(bool blocked);
    void SetIconSize(const Vector2i &size);
    void SetIconTexture(Texture2D *texture);
    void SetIconSpacingWithText(int spacingWithText);
    void SetIcon(Texture2D *texture, const Vector2i &size,
                 int spacingWithText = 0);

    bool IsBlocked() const;
    UIImageRenderer* GetIcon() const;
    UITextRenderer* GetText() const;
    UIImageRenderer* GetBackground() const;
    UIDirLayout *GetDirLayout() const;
    UIFocusable* GetButton() const;

private:
    static const Color IdleColor;
    static const Color OverColor;
    static const Color BlockedColor;
    static const Color PressedColor;

    bool m_isBlocked = false;
    UIImageRenderer *p_icon        = nullptr;
    UITextRenderer  *p_text        = nullptr;
    UIImageRenderer *p_background  = nullptr;
    UIFocusable     *p_button      = nullptr;

    UIButton();
    virtual ~UIButton();

    static UIButton *CreateInto(GameObject *go);

    // IFocusListener
    virtual void OnMouseEnter(IFocusable *focusable) override;
    virtual void OnMouseExit(IFocusable *focusable) override;

    friend class GameObjectFactory;
};

NAMESPACE_BANG_END

#endif // UIBUTTONDRIVER_H
