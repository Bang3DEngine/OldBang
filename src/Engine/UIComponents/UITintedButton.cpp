#include "Bang/UITintedButton.h"

#include "Bang/Material.h"
#include "Bang/GameObject.h"
#include "Bang/UIRenderer.h"

#include "Bang/Renderer.h"

USING_NAMESPACE_BANG

UITintedButton::UITintedButton()
{
    AddListener(this);
}

UITintedButton::~UITintedButton()
{
}

void UITintedButton::OnUpdate()
{
    UIButton::OnUpdate();
    if ( (m_currentTintColor == m_pressedTintColor) &&
         Input::GetMouseButtonUp(MouseButton::Left) )
    {
        ApplyTint( IsMouseOverSomeAgent() ? m_overTintColor : m_idleTintColor);
    }
}

void UITintedButton::AddToTint(Renderer *rend)
{
    p_renderersToTint.Add(rend);
}

void UITintedButton::AddToTint(GameObject *go)
{
    p_gameObjectsToTint.Add(go);
}

void UITintedButton::SetIdleTintColor(const Color &tintColor)
{
    m_idleTintColor = tintColor;
}
void UITintedButton::SetOverTintColor(const Color &tintColor)
{
    m_overTintColor = tintColor;
}
void UITintedButton::SetPressedTintColor(const Color &tintColor)
{
    m_pressedTintColor = tintColor;
}

const Color &UITintedButton::GetIdleTintColor() const
{
    return m_idleTintColor;
}
const Color &UITintedButton::GetOverTintColor() const
{
    return m_overTintColor;
}
const Color &UITintedButton::GetPressedTintColor() const
{
    return m_pressedTintColor;
}

void UITintedButton::OnButton_MouseEnter(UIButton *btn)
{
    UIButtonListener::OnButton_MouseEnter(btn);
    ApplyTint( IsBeingPressed() ? m_pressedTintColor :
                                               m_overTintColor);
}

void UITintedButton::OnButton_MouseExit(UIButton *btn)
{
    UIButtonListener::OnButton_MouseExit(btn);
    ApplyTint( IsBeingPressed() ? m_pressedTintColor : m_idleTintColor);
}

void UITintedButton::OnButton_MouseDown(UIButton *btn,
                                             MouseButton mb)
{
    UIButtonListener::OnButton_MouseDown(btn, mb);
    ApplyTint(m_pressedTintColor);
}

void UITintedButton::OnButton_MouseUp(UIButton *btn,
                                           MouseButton mb)
{
    UIButtonListener::OnButton_MouseUp(btn, mb);
    ApplyTint(m_overTintColor);
}

void UITintedButton::ApplyTint(const Color &tintColor)
{
    List<Renderer*> rendsToTint = p_renderersToTint.GetKeys();
    for (GameObject *go : p_gameObjectsToTint)
    {
        rendsToTint.PushBack( go->GetComponents<Renderer>() );
    }

    for (Renderer *rend : rendsToTint)
    {
        Material *mat = rend->GetMaterial();
        if (mat) { mat->SetDiffuseColor(tintColor); }
    }
    m_currentTintColor = tintColor;
}
