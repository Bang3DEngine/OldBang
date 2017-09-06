#include "Bang/UIButtonTinter.h"

#include "Bang/Material.h"
#include "Bang/GameObject.h"
#include "Bang/UIRenderer.h"

USING_NAMESPACE_BANG

UIButtonTinter::UIButtonTinter()
{
    AddListener(this);
}

UIButtonTinter::~UIButtonTinter()
{
}

void UIButtonTinter::OnUpdate()
{
    UIButton::OnUpdate();
    if ( (m_currentTintColor == m_pressedTintColor) &&
         Input::GetMouseButtonUp(Input::MouseButton::Left) )
    {
        ApplyTintToGameObjects( IsMouseOverSomeAgent() ? m_overTintColor :
                                                         m_idleTintColor);
    }
}

void UIButtonTinter::AddGameObjectToTint(GameObject *go)
{
    p_gameObjectsToTint.Add(go);
}

void UIButtonTinter::SetIdleTintColor(const Color &tintColor)
{
    m_idleTintColor = tintColor;
}
void UIButtonTinter::SetOverTintColor(const Color &tintColor)
{
    m_overTintColor = tintColor;
}
void UIButtonTinter::SetPressedTintColor(const Color &tintColor)
{
    m_pressedTintColor = tintColor;
}

const Color &UIButtonTinter::GetIdleTintColor() const
{
    return m_idleTintColor;
}
const Color &UIButtonTinter::GetOverTintColor() const
{
    return m_overTintColor;
}
const Color &UIButtonTinter::GetPressedTintColor() const
{
    return m_pressedTintColor;
}

void UIButtonTinter::OnButton_MouseEnter(UIButton *btn)
{
    UIButtonListener::OnButton_MouseEnter(btn);
    ApplyTintToGameObjects( IsBeingPressed() ? m_pressedTintColor :
                                               m_overTintColor);
}

void UIButtonTinter::OnButton_MouseExit(UIButton *btn)
{
    UIButtonListener::OnButton_MouseExit(btn);
    ApplyTintToGameObjects( IsBeingPressed() ? m_pressedTintColor :
                                               m_idleTintColor);
}

void UIButtonTinter::OnButton_MouseDown(UIButton *btn, Input::MouseButton mb)
{
    UIButtonListener::OnButton_MouseDown(btn, mb);
    ApplyTintToGameObjects(m_pressedTintColor);
}

void UIButtonTinter::OnButton_MouseUp(UIButton *btn, Input::MouseButton mb)
{
    UIButtonListener::OnButton_MouseUp(btn, mb);
    ApplyTintToGameObjects(m_overTintColor);
}

void UIButtonTinter::ApplyTintToGameObjects(const Color &tintColor)
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


