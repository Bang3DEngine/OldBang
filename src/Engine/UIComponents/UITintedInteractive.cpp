#include "Bang/UITintedInteractive.h"

#include "Bang/Material.h"
#include "Bang/GameObject.h"
#include "Bang/UIRenderer.h"

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
        ApplyTintToGameObjects( IsMouseOverSomeAgent() ? m_overTintColor :
                                                         m_idleTintColor);
    }
}

void UITintedInteractive::AddGameObjectToTint(GameObject *go)
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
    ApplyTintToGameObjects( IsBeingPressed() ? m_pressedTintColor :
                                               m_overTintColor);
}

void UITintedInteractive::OnButton_MouseExit(UIInteractive *btn)
{
    UIInteractiveListener::OnButton_MouseExit(btn);
    ApplyTintToGameObjects( IsBeingPressed() ? m_pressedTintColor :
                                               m_idleTintColor);
}

void UITintedInteractive::OnButton_MouseDown(UIInteractive *btn, Input::MouseButton mb)
{
    UIInteractiveListener::OnButton_MouseDown(btn, mb);
    ApplyTintToGameObjects(m_pressedTintColor);
}

void UITintedInteractive::OnButton_MouseUp(UIInteractive *btn, Input::MouseButton mb)
{
    UIInteractiveListener::OnButton_MouseUp(btn, mb);
    ApplyTintToGameObjects(m_overTintColor);
}

void UITintedInteractive::ApplyTintToGameObjects(const Color &tintColor)
{
    for (GameObject *go : p_gameObjectsToTint)
    {
        List<UIRenderer*> uiRenderers = go->GetComponents<UIRenderer>();
        for (UIRenderer *uiRend : uiRenderers)
        {
            Material *mat = uiRend->GetMaterial();
            if (mat) { mat->SetDiffuseColor(tintColor); }
        }
    }
    m_currentTintColor = tintColor;
}
