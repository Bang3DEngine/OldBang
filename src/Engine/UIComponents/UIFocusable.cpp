#include "Bang/UIFocusable.h"

#include "Bang/Rect.h"
#include "Bang/List.h"
#include "Bang/Input.h"
#include "Bang/GameObject.h"
#include "Bang/RectTransform.h"
#include "Bang/IFocusListener.h"

USING_NAMESPACE_BANG

UIFocusable::UIFocusable()
{
}

UIFocusable::~UIFocusable()
{
}

void UIFocusable::OnUpdate()
{
    Component::OnUpdate();
}

void UIFocusable::OnPostUpdate()
{
    Component::OnPostUpdate();
    m_hasJustFocusChanged = false;
}

bool UIFocusable::HasMouseOver() const
{
    return m_hasMouseOver;
}

void UIFocusable::SetFocusEnabled(bool focusEnabled)
{
    m_focusEnabled = focusEnabled;
}

bool UIFocusable::HasFocus() const { return m_hasFocus; }
bool UIFocusable::IsFocusEnabled() const { return m_focusEnabled; }
bool UIFocusable::HasJustFocusChanged() const { return m_hasJustFocusChanged; }
bool UIFocusable::CanBeRepeatedInGameObject() const { return false; }

void UIFocusable::SetFocus()
{
    ASSERT(IsFocusEnabled());
    if (!HasFocus())
    {
        m_hasFocus = true;
        m_hasJustFocusChanged = true;
        PropagateToFocusListeners();
    }
}

void UIFocusable::ClearFocus()
{
    if (HasFocus())
    {
        m_hasFocus = false;
        m_hasJustFocusChanged = true;
        PropagateToFocusListeners();
    }
}

void UIFocusable::PropagateToFocusListeners()
{
    if (HasFocus())
    {
        EventEmitter<IFocusListener>::
                PropagateToListeners(&IFocusListener::OnFocusTaken);
        GameObject::Propagate(&IFocusListener::OnFocusTaken,
                              GetGameObject()->GetComponents<IFocusListener>());
    }
    else
    {
        EventEmitter<IFocusListener>::
            PropagateToListeners(&IFocusListener::OnFocusLost);
        GameObject::Propagate(&IFocusListener::OnFocusLost,
                              GetGameObject()->GetComponents<IFocusListener>());
    }
}

