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

bool UIFocusable::HasJustFocusChanged() const { return m_hasJustFocusChanged; }
bool UIFocusable::CanBeRepeatedInGameObject() const { return false; }

void UIFocusable::SetFocus()
{
    if (!HasFocus()) { m_hasJustFocusChanged = true; }
    IFocusable::SetFocus();
}

void UIFocusable::ClearFocus()
{
    if (!HasFocus()) { m_hasJustFocusChanged = true; }
    IFocusable::ClearFocus();
}

void UIFocusable::PropagateToFocusListeners()
{
    IFocusable::PropagateToFocusListeners();

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


