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

    m_hasJustFocusChanged = false;
    HandleFocusing(false);
    HandleFocusing(true);
}

bool UIFocusable::IsMouseOver() const
{
    return GetGameObject()->GetRectTransform()->IsMouseOver();
}

void UIFocusable::LeaveFocus()
{
    if (HasFocus())
    {
        m_hasFocus = false;
        m_hasJustFocusChanged = true;
        PropagateToFocusListeners();
    }
}
bool UIFocusable::HasFocus() const { return m_hasFocus; }
bool UIFocusable::HasJustFocusChanged() const { return m_hasJustFocusChanged; }

void UIFocusable::HandleFocusing(bool handleFocusTake)
{
    if (Input::GetMouseButtonDown(MouseButton::Left))
    {
        bool gotFocus = IsMouseOver();
        bool triggerEvent = (HasFocus() != gotFocus);
        if (triggerEvent && (gotFocus == handleFocusTake))
        {
            m_hasFocus = gotFocus;
            m_hasJustFocusChanged = true;
            PropagateToFocusListeners();
        }
    }
}

void UIFocusable::PropagateToFocusListeners()
{
    if (HasFocus())
    {
        PROPAGATE_1(IFocusListener, OnFocusTaken,
                    GetGameObject()->GetComponents<IFocusListener>());
    }
    else
    {
        PROPAGATE_1(IFocusListener, OnFocusLost,
                    GetGameObject()->GetComponents<IFocusListener>());
    }
}

