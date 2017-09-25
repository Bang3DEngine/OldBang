#include "Bang/UITintedInteractive.h"

#include "Bang/Material.h"
#include "Bang/GameObject.h"
#include "Bang/UIRenderer.h"

#include "Bang/Renderer.h"

USING_NAMESPACE_BANG

UITintedInteractive::UITintedInteractive()
{
    AddListener(this);
}

UITintedInteractive::~UITintedInteractive()
{
}

void UITintedInteractive::OnUpdate()
{
    UIInteractive::OnUpdate();
    if ( (m_currentTintColor == m_pressedTintColor) &&
         Input::GetMouseButtonUp(Input::MouseButton::Left) )
    {
        ApplyTint( IsMouseOverSomeAgent() ? m_overTintColor : m_idleTintColor);
    }
}

void UITintedInteractive::AddToTint(Renderer *rend)
{
    p_renderersToTint.Add(rend);
}

void UITintedInteractive::AddToTint(GameObject *go)
{
    p_gameObjectsToTint.Add(go);
}

void UITintedInteractive::SetIdleTintColor(const Color &tintColor)
{
    m_idleTintColor = tintColor;
}
void UITintedInteractive::SetOverTintColor(const Color &tintColor)
{
    m_overTintColor = tintColor;
}
void UITintedInteractive::SetPressedTintColor(const Color &tintColor)
{
    m_pressedTintColor = tintColor;
}

const Color &UITintedInteractive::GetIdleTintColor() const
{
    return m_idleTintColor;
}
const Color &UITintedInteractive::GetOverTintColor() const
{
    return m_overTintColor;
}
const Color &UITintedInteractive::GetPressedTintColor() const
{
    return m_pressedTintColor;
}

void UITintedInteractive::OnButton_MouseEnter(UIInteractive *btn)
{
    UIInteractiveListener::OnButton_MouseEnter(btn);
    ApplyTint( IsBeingPressed() ? m_pressedTintColor :
                                               m_overTintColor);
}

void UITintedInteractive::OnButton_MouseExit(UIInteractive *btn)
{
    UIInteractiveListener::OnButton_MouseExit(btn);
    ApplyTint( IsBeingPressed() ? m_pressedTintColor : m_idleTintColor);
}

void UITintedInteractive::OnButton_MouseDown(UIInteractive *btn,
                                             Input::MouseButton mb)
{
    UIInteractiveListener::OnButton_MouseDown(btn, mb);
    ApplyTint(m_pressedTintColor);
}

void UITintedInteractive::OnButton_MouseUp(UIInteractive *btn,
                                           Input::MouseButton mb)
{
    UIInteractiveListener::OnButton_MouseUp(btn, mb);
    ApplyTint(m_overTintColor);
}

void UITintedInteractive::ApplyTint(const Color &tintColor)
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
