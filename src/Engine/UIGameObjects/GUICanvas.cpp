#include "Bang/GUICanvas.h"

#include "Bang/Input.h"
#include "Bang/XMLNode.h"

GUICanvas::GUICanvas() : UIGameObject("Canvas")
{
}

GUICanvas::~GUICanvas()
{
}

void GUICanvas::OnUpdate()
{
    UIGameObject::OnUpdate();

    if (Input::GetMouseButtonDown(Input::MouseButton::Left) ||
        Input::GetMouseButtonDown(Input::MouseButton::Right) )
    {
        Vector2 mouseCoordsNDC = Input::GetMouseCoordsNDC();
        UIGameObject *focusTaker = PropagateFocus(mouseCoordsNDC);
        GiveFocusTo(focusTaker);
    }
}

void GUICanvas::CloneInto(ICloneable *clone) const
{
    UIGameObject::CloneInto(clone);
}

void GUICanvas::Read(const XMLNode &xmlInfo)
{
    UIGameObject::Read(xmlInfo);
}

void GUICanvas::Write(XMLNode *xmlInfo) const
{
    UIGameObject::Write(xmlInfo);
}

UIGameObject *GUICanvas::GetFocusedGameObject() const
{
    return p_focus;
}

void GUICanvas::GiveFocusTo(UIGameObject *focusTaker) const
{
    if (p_focus)
    {
        p_focus->m_hasFocus = false;
        p_focus->OnFocusLost();
    }

    p_focus = focusTaker;
    if (p_focus)
    {
        p_focus->m_hasFocus = true;
        p_focus->OnFocusTaken();
    }
}
