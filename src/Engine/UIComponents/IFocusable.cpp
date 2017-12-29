#include "Bang/IFocusable.h"

#include "Bang/Input.h"

USING_NAMESPACE_BANG

IFocusable::IFocusable()
{
}

IFocusable::~IFocusable()
{
}

void IFocusable::UpdateFromCanvas()
{
    // Mouse Down event
    if (IsMouseOver())
    {
        if (Input::GetMouseButtonDown(MouseButton::Left))
        {
            m_beingPressed = true;
            Click(false);
        }

        if ( Input::GetMouseButtonDoubleClick(MouseButton::Left) )
        {
            Click(true);
        }
    }

    if (!Input::GetMouseButton(MouseButton::Left))
    {
        m_beingPressed = false;
    }
}

bool IFocusable::IsBeingPressed() const { return m_beingPressed; }
void IFocusable::AddClickedCallback(ClickedCallback callback)
{
    m_clickedCallbacks.PushBack(callback);
}

void IFocusable::AddDoubleClickedCallback(IFocusable::ClickedCallback callback)
{
    m_doubleClickedCallbacks.PushBack(callback);
}


bool IFocusable::IsMouseOver() const
{
    return m_isMouseOver;
}

void IFocusable::SetFocusEnabled(bool focusEnabled)
{
    m_focusEnabled = focusEnabled;
}

void IFocusable::Click(bool doubleClick)
{
    PropagateOnClickedToListeners(doubleClick);
}

bool IFocusable::HasFocus() const { return m_hasFocus; }
bool IFocusable::IsFocusEnabled() const { return m_focusEnabled; }
bool IFocusable::HasJustFocusChanged() const { return m_hasJustFocusChanged; }

void IFocusable::SetFocus()
{
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

void IFocusable::PropagateOnClickedToListeners(bool doubleClick)
{
    if (IsEmittingEvents())
    {
        if (!doubleClick)
        {
            EventEmitter<IFocusListener>::
                PropagateToListeners(&IFocusListener::OnClicked, this);
            for (auto callback : m_clickedCallbacks) { callback(this); }
        }
        else
        {
            EventEmitter<IFocusListener>::
                PropagateToListeners(&IFocusListener::OnDoubleClicked, this);
            for (auto callback : m_doubleClickedCallbacks) { callback(this); }
        }
    }
}
