#include "Bang/UIGameObject.h"

#include "Bang/Rect.h"
#include "Bang/Input.h"
#include "Bang/RectTransform.h"

UIGameObject::UIGameObject(const String& name) : GameObject(name)
{
    m_rectTransform = AddComponent<RectTransform>();
}

bool UIGameObject::IsMouseOver() const
{
    Vector2 mouseCoordsNDC = Input::GetMouseCoordsNDC();
    return rectTransform->GetScreenSpaceRectNDC().Contains(mouseCoordsNDC);
}

bool UIGameObject::HasFocus() const
{
    return m_hasFocus;
}

void UIGameObject::SetDefaultFocusAction(
                            UIGameObject::FocusAction defaultFocusAction)
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

        Rect ndcRect = uiChild->rectTransform->GetScreenSpaceRectNDC();
        if (ndcRect.Contains(mouseCoordsNDC))
        {
            return uiChild->PropagateFocus(mouseCoordsNDC);
        }
    }
    return ReceiveFocus();
}

UIGameObject::FocusAction UIGameObject::OnFocusReceived()
{
    return m_defaultFocusAction;
}
void UIGameObject::OnFocusTaken() {}
void UIGameObject::OnFocusLost() {}

