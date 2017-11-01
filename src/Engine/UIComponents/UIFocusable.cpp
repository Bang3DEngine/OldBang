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

void UIFocusable::OnPreUpdate()
{
    Component::OnPreUpdate();

    m_hasJustFocusChanged = false;
    HandleFocusing(false);
}

void UIFocusable::OnUpdate()
{
    Component::OnUpdate();

    HandleFocusing(true);
}

bool UIFocusable::HasFocus() const { return m_hasFocus; }
bool UIFocusable::HasJustFocusChanged() const { return m_hasJustFocusChanged; }

void UIFocusable::HandleFocusing(bool handleFocusTake)
{
    bool mouseOver = GetGameObject()->GetComponent<RectTransform>()->IsMouseOver();
    if (Input::GetMouseButtonDown(MouseButton::Left))
    {
        bool gotFocus = mouseOver;
        bool triggerEvent = (HasFocus() != gotFocus);
        if (triggerEvent && (gotFocus == handleFocusTake))
        {
            m_hasFocus = gotFocus;
            m_hasJustFocusChanged = true;

            List<IFocusListener*> focusListeners =
                            GetGameObject()->GetComponents<IFocusListener>();
            for (IFocusListener *fl : focusListeners)
            {
                if (gotFocus) { fl->OnFocusTaken(); }
                else { fl->OnFocusLost();  }
            }
        }
    }
}

