#include "Bang/UITintedButton.h"

#include "Bang/Material.h"
#include "Bang/GameObject.h"
#include "Bang/UIRenderer.h"

#include "Bang/Renderer.h"

USING_NAMESPACE_BANG

UITintedButton::UITintedButton()
{
    EventEmitter<IUIButtonListener>::RegisterListener(this);
}

UITintedButton::~UITintedButton()
{
}

void UITintedButton::OnUpdate()
{
    UIButton::OnUpdate();
    if ( Input::GetMouseButtonUp(MouseButton::Left) )
    {
        ApplyTint( IsMouseOverSomeEmitter() ? m_overTintColor : m_idleTintColor);
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

void UITintedButton::SetTintEnabled(bool tintEnabled)
{
    m_tintEnabled = tintEnabled;
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

bool UITintedButton::IsTintEnabled() const
{
    return m_tintEnabled;
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
    IUIButtonListener::OnButton_MouseEnter(btn);
    ApplyTint( IsBeingPressed() ? m_pressedTintColor : m_overTintColor);
}

void UITintedButton::OnButton_MouseExit(UIButton *btn)
{
    IUIButtonListener::OnButton_MouseExit(btn);
    ApplyTint( IsBeingPressed() ? m_pressedTintColor : m_idleTintColor);
}

void UITintedButton::OnButton_MouseDown(UIButton *btn,
                                             MouseButton mb)
{
    IUIButtonListener::OnButton_MouseDown(btn, mb);
    ApplyTint(m_pressedTintColor);
}

void UITintedButton::OnButton_MouseUp(UIButton *btn,
                                           MouseButton mb)
{
    IUIButtonListener::OnButton_MouseUp(btn, mb);
    ApplyTint(m_overTintColor);
}

void UITintedButton::ApplyTint(const Color &tintColor)
{
    if (IsTintEnabled())
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
    }
}
