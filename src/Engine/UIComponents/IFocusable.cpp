#include "Bang/IFocusable.h"

#include "Bang/Component.h"

USING_NAMESPACE_BANG

IFocusable::IFocusable()
{
}

IFocusable::~IFocusable()
{
}

bool IFocusable::HasMouseOver() const
{
    return m_hasMouseOver;
}

void IFocusable::SetFocusEnabled(bool focusEnabled)
{
    m_focusEnabled = focusEnabled;
}

bool IFocusable::HasFocus() const { return m_hasFocus; }
bool IFocusable::IsFocusEnabled() const { return m_focusEnabled; }

void IFocusable::SetFocus()
{
    ASSERT(IsFocusEnabled());
    if (!HasFocus())
    {
        m_hasFocus = true;
        PropagateToFocusListeners();
    }
}

void IFocusable::ClearFocus()
{
    if (HasFocus())
    {
        m_hasFocus = false;
        PropagateToFocusListeners();
    }
}

void IFocusable::PropagateToFocusListeners()
{
    if (HasFocus())
    {
        EventEmitter<IFocusListener>::
                PropagateToListeners(&IFocusListener::OnFocusTaken);
    }
    else
    {
        EventEmitter<IFocusListener>::
            PropagateToListeners(&IFocusListener::OnFocusLost);
    }
}
