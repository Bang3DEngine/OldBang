#ifndef UITINTEDINTERACTIVE_H
#define UITINTEDINTERACTIVE_H

#include "Bang/Color.h"
#include "Bang/UIInteractive.h"

NAMESPACE_BANG_BEGIN

class UITintedInteractive : public UIInteractive,
                            public UIInteractiveListener
{
    COMPONENT(UITintedInteractive)

public:
    UITintedInteractive();
    virtual ~UITintedInteractive();

    // Component
    virtual void OnUpdate() override;

    void AddGameObjectToTint(GameObject *go);

    void SetOverTintColor(const Color &tintColor);
    void SetIdleTintColor(const Color &tintColor);
    void SetPressedTintColor(const Color &tintColor);

    const Color& GetIdleTintColor() const;
    const Color& GetOverTintColor() const;
    const Color& GetPressedTintColor() const;

protected:
    virtual void OnButton_MouseEnter(UIInteractive *btn) override;
    virtual void OnButton_MouseExit(UIInteractive *btn) override;
    virtual void
    OnButton_MouseDown(UIInteractive *btn, Input::MouseButton mb) override;
    virtual void
    OnButton_MouseUp(UIInteractive *btn, Input::MouseButton mb) override;

private:
    Set<GameObject*> p_gameObjectsToTint;
    Color m_idleTintColor    = Color::White;
    Color m_overTintColor    = Color::LightBlue;
    Color m_pressedTintColor = Color::DarkBlue;

    Color m_currentTintColor = m_idleTintColor;

    void ApplyTintToGameObjects(const Color &tintColor);
};

NAMESPACE_BANG_END

#endif // UITINTEDINTERACTIVE_H
