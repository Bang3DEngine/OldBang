#include "Bang/UICanvas.h"

#include "Bang/Input.h"
#include "Bang/XMLNode.h"
#include "Bang/UIGameObject.h"

UICanvas::UICanvas()
{
}

UICanvas::~UICanvas()
{
}

void UICanvas::OnUpdate()
{
    Component::OnUpdate();

    if (Input::GetMouseButtonDown(Input::MouseButton::Left) ||
        Input::GetMouseButtonDown(Input::MouseButton::Right) )
    {
        Vector2 mouseCoordsNDC = Input::GetMouseCoordsNDC();
        UIGameObject *uiGo = SCAST<UIGameObject*>(GetGameObject());
        UIGameObject *focusTaker = uiGo->PropagateFocus(mouseCoordsNDC);
        GiveFocusTo(focusTaker);
    }
}

void UICanvas::CloneInto(ICloneable *clone) const
{
    Component::CloneInto(clone);
}

void UICanvas::Read(const XMLNode &xmlInfo)
{
    Component::Read(xmlInfo);
}

void UICanvas::Write(XMLNode *xmlInfo) const
{
    Component::Write(xmlInfo);
}

UIGameObject *UICanvas::GetFocusedGameObject() const
{
    return p_focus;
}

void UICanvas::GiveFocusTo(UIGameObject *focusTaker) const
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
