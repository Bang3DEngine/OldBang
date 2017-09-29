#ifndef UITINTEDBUTTON_H
#define UITINTEDBUTTON_H

#include "Bang/Color.h"
#include "Bang/UIButton.h"

NAMESPACE_BANG_BEGIN

FORWARD class Renderer;

class UITintedButton : public UIButton,
                       public UIButtonListener
{
    COMPONENT(UITintedButton)

public:
    UITintedButton();
    virtual ~UITintedButton();

    // Component
    virtual void OnUpdate() override;

    void AddToTint(Renderer *rnd);
    void AddToTint(GameObject *go);

    void SetOverTintColor(const Color &tintColor);
    void SetIdleTintColor(const Color &tintColor);
    void SetPressedTintColor(const Color &tintColor);

    const Color& GetIdleTintColor() const;
    const Color& GetOverTintColor() const;
    const Color& GetPressedTintColor() const;

protected:
    virtual void OnButton_MouseEnter(UIButton *btn) override;
    virtual void OnButton_MouseExit(UIButton *btn) override;
    virtual void
    OnButton_MouseDown(UIButton *btn, MouseButton mb) override;
    virtual void
    OnButton_MouseUp(UIButton *btn, MouseButton mb) override;

private:
    Set<Renderer*>   p_renderersToTint;
    Set<GameObject*> p_gameObjectsToTint;
    Color m_idleTintColor    = Color::White;
    Color m_overTintColor    = Color::LightBlue;
    Color m_pressedTintColor = Color::DarkBlue;

    Color m_currentTintColor = m_idleTintColor;

    void ApplyTint(const Color &tintColor);
};

NAMESPACE_BANG_END

#endif // UITINTEDBUTTON_H