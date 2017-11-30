#include "Bang/IFocusable.h"

USING_NAMESPACE_BANG

IFocusable::IFocusable()
{
}

IFocusable::~IFocusable()
{
}

bool IFocusable::IsMouseOver() const
{
    return m_isMouseOver;
}

void IFocusable::SetFocusEnabled(bool focusEnabled)
{
    m_focusEnabled = focusEnabled;
}

bool IFocusable::HasFocus() const { return m_hasFocus; }
bool IFocusable::IsFocusEnabled() const { return m_focusEnabled; }
bool IFocusable::HasJustFocusChanged() const { return m_hasJustFocusChanged; }

void IFocusable::SetFocus()
{
    ASSERT(IsFocusEnabled());
    if (!HasFocus())
    {
        m_hasFocus = true;
        m_hasJustFocusChanged = true;
        PropagateFocusToListeners();
    }
}

void IFocusable::ClearFocus()
{
    if (HasFocus())
    {
        m_hasFocus = false;
        m_hasJustFocusChanged = true;
        PropagateFocusToListeners();
    }
}

void IFocusable::PropagateMouseOverToListeners(bool mouseOver)
{
    if (IsMouseOver() != mouseOver)
    {
        m_isMouseOver = mouseOver;
        if (IsMouseOver())
        {
            EventEmitter<IFocusListener>::
                    PropagateToListeners(&IFocusListener::OnMouseEnter, this);
        }
        else
        {
            EventEmitter<IFocusListener>::
                    PropagateToListeners(&IFocusListener::OnMouseExit, this);
        }
    }
}

void IFocusable::PropagateFocusToListeners()
{
    if (HasFocus())
    {
        EventEmitter<IFocusListener>::
                PropagateToListeners(&IFocusListener::OnFocusTaken, this);
    }
    else
    {
        EventEmitter<IFocusListener>::
            PropagateToListeners(&IFocusListener::OnFocusLost, this);
    }
}
