#include "Bang/UICanvas.h"

#include "Bang/Input.h"
#include "Bang/XMLNode.h"
#include "Bang/GameObject.h"
#include "Bang/UIFocusTaker.h"

USING_NAMESPACE_BANG

UICanvas::UICanvas()
{
}

UICanvas::~UICanvas()
{
}

void UICanvas::OnUpdate()
{
    Component::OnUpdate();

    if (Input::GetMouseButtonDown(MouseButton::Left) ||
        Input::GetMouseButtonDown(MouseButton::Right) )
    {
        Vector2 mouseCoordsNDC = Input::GetMouseCoordsNDC();
        UIFocusTaker *canvasFocusTaker = gameObject->GetComponent<UIFocusTaker>();
        if (canvasFocusTaker)
        {
            UIFocusTaker *focusTaker = canvasFocusTaker->PropagateFocus(mouseCoordsNDC);
            if (focusTaker) { GiveFocusTo(focusTaker->gameObject); }
        }
    }
}

void UICanvas::CloneInto(ICloneable *clone) const
{
    Component::CloneInto(clone);
}

void UICanvas::ImportXML(const XMLNode &xmlInfo)
{
    Component::ImportXML(xmlInfo);
}

void UICanvas::ExportXML(XMLNode *xmlInfo) const
{
    Component::ExportXML(xmlInfo);
}

GameObject *UICanvas::GetFocusedGameObject() const
{
    return p_focusedGo;
}

void UICanvas::GiveFocusTo(GameObject *focusTakerGo) const
{
    if (p_focusedGo == focusTakerGo) { return; }

    if (p_focusedGo)
    {
        UIFocusTaker *focusTaker = p_focusedGo->GetComponent<UIFocusTaker>();
        focusTaker->m_hasFocus = false;
        focusTaker->OnFocusLost();
    }

    p_focusedGo = focusTakerGo;
    if (p_focusedGo)
    {
        UIFocusTaker *focusTaker = p_focusedGo->GetComponent<UIFocusTaker>();
        focusTaker->m_hasFocus = true;
        focusTaker->OnFocusTaken();
    }
}
