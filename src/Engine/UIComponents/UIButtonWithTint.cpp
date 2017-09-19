#include "Bang/UIButtonWithTint.h"

#include "Bang/Material.h"
#include "Bang/GameObject.h"
#include "Bang/UIRenderer.h"

USING_NAMESPACE_BANG

UIButtonWithTint::UIButtonWithTint()
{
    AddListener(this);
}

UIButtonWithTint::~UIButtonWithTint()
{
}

void UIButtonWithTint::OnUpdate()
{
    UIButton::OnUpdate();
    if ( (m_currentTintColor == m_pressedTintColor) &&
         Input::GetMouseButtonUp(Input::MouseButton::Left) )
    {
        ApplyTintToGameObjects( IsMouseOverSomeAgent() ? m_overTintColor :
                                                         m_idleTintColor);
    }
}

void UIButtonWithTint::AddGameObjectToTint(GameObject *go)
{
    p_gameObjectsToTint.Add(go);
}

void UIButtonWithTint::SetIdleTintColor(const Color &tintColor)
{
    m_idleTintColor = tintColor;
}
void UIButtonWithTint::SetOverTintColor(const Color &tintColor)
{
    m_overTintColor = tintColor;
}
void UIButtonWithTint::SetPressedTintColor(const Color &tintColor)
{
    m_pressedTintColor = tintColor;
}

const Color &UIButtonWithTint::GetIdleTintColor() const
{
    return m_idleTintColor;
}
const Color &UIButtonWithTint::GetOverTintColor() const
{
    return m_overTintColor;
}
const Color &UIButtonWithTint::GetPressedTintColor() const
{
    return m_pressedTintColor;
}

void UIButtonWithTint::OnButton_MouseEnter(UIButton *btn)
{
    UIButtonListener::OnButton_MouseEnter(btn);
    ApplyTintToGameObjects( IsBeingPressed() ? m_pressedTintColor :
                                               m_overTintColor);
}

void UIButtonWithTint::OnButton_MouseExit(UIButton *btn)
{
    UIButtonListener::OnButton_MouseExit(btn);
    ApplyTintToGameObjects( IsBeingPressed() ? m_pressedTintColor :
                                               m_idleTintColor);
}

void UIButtonWithTint::OnButton_MouseDown(UIButton *btn, Input::MouseButton mb)
{
    UIButtonListener::OnButton_MouseDown(btn, mb);
    ApplyTintToGameObjects(m_pressedTintColor);
}

void UIButtonWithTint::OnButton_MouseUp(UIButton *btn, Input::MouseButton mb)
{
    UIButtonListener::OnButton_MouseUp(btn, mb);
    ApplyTintToGameObjects(m_overTintColor);
}

void UIButtonWithTint::ApplyTintToGameObjects(const Color &tintColor)
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
