#include "Bang/UIFocusTaker.h"

#include "Bang/Rect.h"
#include "Bang/Input.h"
#include "Bang/Vector2.h"
#include "Bang/XMLNode.h"
#include "Bang/GameObject.h"
#include "Bang/RectTransform.h"

USING_NAMESPACE_BANG

UIFocusTaker::UIFocusTaker()
{

}

UIFocusTaker::~UIFocusTaker()
{

}

UIFocusTaker* UIFocusTaker::ReceiveFocus()
{
    FocusAction focusAction = OnFocusReceived();
    if (focusAction == FocusAction::TakeIt)
    {
        return this;
    }
    else if (focusAction == FocusAction::PassToParent && gameObject->parent)
    {
        if (gameObject->parent)
        {
            UIFocusTaker *focusTaker = gameObject->parent->GetComponent<UIFocusTaker>();
            if (focusTaker) { return focusTaker->ReceiveFocus(); }
        }
    }
    return nullptr;
}

UIFocusTaker* UIFocusTaker::PropagateFocus(const Vector2 &mouseCoordsNDC)
{
    List<GameObject*> children = gameObject->GetChildren();
    for (GameObject *child : children)
    {
        RectTransform *rt = child->GetComponent<RectTransform>();
        if (rt)
        {
            Rect ndcRect = rt->GetScreenSpaceRectNDC();
            if (ndcRect.Contains(mouseCoordsNDC))
            {
                UIFocusTaker *ft = child->GetComponent<UIFocusTaker>();
                if (ft) { return ft->PropagateFocus(mouseCoordsNDC); }
            }
        }
    }

    return ReceiveFocus();
}

FocusAction UIFocusTaker::OnFocusReceived()
{
    return m_defaultFocusAction;
}

void UIFocusTaker::OnFocusTaken()
{
    IFocusListener::OnFocusTaken();
    List<IFocusListener*> focusListeners = gameObject->GetComponents<IFocusListener>();
    for (IFocusListener *focusListener : focusListeners)
    {
        if (focusListener != this) { focusListener->OnFocusTaken(); }
    }
}

void UIFocusTaker::OnFocusLost()
{
    IFocusListener::OnFocusLost();
    List<IFocusListener*> focusListeners = gameObject->GetComponents<IFocusListener>();
    for (IFocusListener *focusListener : focusListeners)
    {
        if (focusListener != this) { focusListener->OnFocusLost(); }
    }
}

void UIFocusTaker::ImportXML(const XMLNode &xmlInfo)
{
    Component::ImportXML(xmlInfo);

    if (xmlInfo.Contains("FocusAction"))
    { SetDefaultFocusAction( xmlInfo.Get<FocusAction>("FocusAction") ); }
}

void UIFocusTaker::ExportXML(XMLNode *xmlInfo) const
{
    Component::ExportXML(xmlInfo);

    xmlInfo->Set("FocusAction", m_defaultFocusAction);
}

void UIFocusTaker::SetDefaultFocusAction(FocusAction focusAction)
{
    m_defaultFocusAction = focusAction;
}

bool UIFocusTaker::IsMouseOver() const
{
    RectTransform *rt = gameObject->GetComponent<RectTransform>();
    if (!rt) { return false; }

    Vector2 mouseCoordsNDC = Input::GetMouseCoordsNDC();
    return rt->GetScreenSpaceRectNDC().Contains(mouseCoordsNDC);
}

bool UIFocusTaker::HasFocus() const
{
    return m_hasFocus;
}
