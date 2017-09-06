#include "Bang/UIGameObject.h"

#include "Bang/Rect.h"
#include "Bang/Input.h"
#include "Bang/XMLNode.h"
#include "Bang/Component.h"
#include "Bang/RectTransform.h"

USING_NAMESPACE_BANG

UIGameObject::UIGameObject(const String& name) : GameObject(name)
{
}

bool UIGameObject::IsMouseOver() const
{
    Vector2 mouseCoordsNDC = Input::GetMouseCoordsNDC();
    return GetRectTransform()->GetScreenSpaceRectNDC().Contains(mouseCoordsNDC);
}

bool UIGameObject::HasFocus() const
{
    return m_hasFocus;
}

RectTransform *UIGameObject::GetRectTransform() const
{
    return transform ? DCAST<RectTransform*>(transform) : nullptr;
}

void UIGameObject::SetDefaultFocusAction(FocusAction defaultFocusAction)
{
    m_defaultFocusAction = defaultFocusAction;
}

UIGameObject* UIGameObject::ReceiveFocus()
{
    FocusAction focusAction = OnFocusReceived();
    if (focusAction == FocusAction::TakeIt)
    {
        return this;
    }
    else if (focusAction == FocusAction::PassToParent && parent)
    {
        UIGameObject *parentUigo = DCAST<UIGameObject*>( parent );
        if (parentUigo)
        {
            return parentUigo->ReceiveFocus();
        }
    }
    return nullptr;
}

UIGameObject* UIGameObject::PropagateFocus(const Vector2 &mouseCoordsNDC)
{
    List<GameObject*> children = GetChildren();
    for (GameObject *child : children)
    {
        UIGameObject *uiChild = DCAST<UIGameObject*>(child);
        if (!uiChild) { continue; }

        Rect ndcRect = uiChild->GetRectTransform()->GetScreenSpaceRectNDC();
        if (ndcRect.Contains(mouseCoordsNDC))
        {
            return uiChild->PropagateFocus(mouseCoordsNDC);
        }
    }

    return ReceiveFocus();
}

FocusAction UIGameObject::OnFocusReceived()
{
    return m_defaultFocusAction;
}

void UIGameObject::OnFocusTaken()
{
    IFocusListener::OnFocusTaken();
    List<IFocusListener*> focusListeners = GetComponents<IFocusListener>();
    for (IFocusListener *focusListener : focusListeners)
    {
        focusListener->OnFocusTaken();
    }
}

void UIGameObject::OnFocusLost()
{
    IFocusListener::OnFocusLost();
    List<IFocusListener*> focusListeners = GetComponents<IFocusListener>();
    for (IFocusListener *focusListener : focusListeners)
    {
        focusListener->OnFocusLost();
    }
}

void UIGameObject::ImportXML(const XMLNode &xmlInfo)
{
    GameObject::ImportXML(xmlInfo);

    if (xmlInfo.Contains("FocusAction"))
    { SetDefaultFocusAction( xmlInfo.Get<FocusAction>("FocusAction") ); }
}

void UIGameObject::ExportXML(XMLNode *xmlInfo) const
{
    GameObject::ExportXML(xmlInfo);
    xmlInfo->SetTagName(UIGameObject::GetClassNameStatic());

    xmlInfo->Set("FocusAction", m_defaultFocusAction);
}


