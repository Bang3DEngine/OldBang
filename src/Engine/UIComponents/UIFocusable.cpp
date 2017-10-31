#include "Bang/UIFocusable.h"

#include "Bang/List.h"
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

void UIFocusable::OnPreUpdate()
{
    Component::OnPreUpdate();
    HandleFocusing(false);
}

void UIFocusable::OnUpdate()
{
    Component::OnUpdate();
    HandleFocusing(true);
}

bool UIFocusable::HasFocus() const { return m_hasFocus; }

void UIFocusable::HandleFocusing(bool focusTake)
{
    bool mouseOver = GetGameObject()->GetComponent<RectTransform>()->IsMouseOver();
    if (Input::GetMouseButtonDown(MouseButton::Left))
    {
        bool hasFocus = mouseOver;
        bool triggerEvent = (HasFocus() != hasFocus);
        if (triggerEvent)
        {
            m_hasFocus = hasFocus;

            List<IFocusListener*> focusListeners =
                            GetGameObject()->GetComponents<IFocusListener>();
            for (IFocusListener *fl : focusListeners)
            {
                if ( HasFocus() &&  focusTake) { fl->OnFocusTaken(); }
                if (!HasFocus() && !focusTake) { fl->OnFocusLost(); }
            }
        }
    }
}

