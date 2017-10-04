#include "Bang/UIListDriver.h"

#include "Bang/Rect.h"
#include "Bang/Input.h"
#include "Bang/GameObject.h"
#include "Bang/UIFocusTaker.h"
#include "Bang/RectTransform.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UIVerticalLayout.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/UIHorizontalLayout.h"

USING_NAMESPACE_BANG

UIListDriver::UIListDriver()
{
}

UIListDriver::~UIListDriver()
{
}

void UIListDriver::OnUpdate()
{
    UIFocusTaker *ft = gameObject->GetComponent<UIFocusTaker>();

    // Selection In/Out
    if (Input::GetKeyDownRepeat(Key::Down))
    {
        SetSelection( GetSelectedIndex() + 1 );
    }

    if (Input::GetKeyDownRepeat(Key::Up))
    {
        SetSelection( GetSelectedIndex() - 1 );
    }

    // Mouse In/Out
    GameObject *mouseOverGo = nullptr;
    Vector2 mouseCoords = Input::GetMouseCoordsNDC();
    for (GameObject *child : gameObject->GetChildren())
    {
        RectTransform *rt = child->GetComponent<RectTransform>();
        if (rt && rt->GetScreenSpaceRectNDC().Contains(mouseCoords))
        {
            mouseOverGo = child; break;
        }
    }

    if (p_mouseOverGo != mouseOverGo)
    {
        if (p_mouseOverGo) { Callback(p_mouseOverGo, Action::MouseOut); }

        p_mouseOverGo = mouseOverGo;
        if (p_mouseOverGo) { Callback(p_mouseOverGo, Action::MouseOver); }
    }

    // Clicked
    if (Input::GetKeyDownRepeat(Key::Right) ||
        Input::GetKeyDownRepeat(Key::Enter))
    {
        GameObject *selectedGo = GetSelectedGameObject();
        if (selectedGo) { Callback(selectedGo, Action::Pressed); }
    }

    if (p_mouseOverGo)
    {
        if (Input::GetMouseButtonDown(MouseButton::Left))
        {
            SetSelection(p_mouseOverGo);
            Callback(p_mouseOverGo, Action::ClickedLeft);
        }

        if (Input::GetMouseButtonDown(MouseButton::Right))
        {
            Callback(p_mouseOverGo, Action::ClickedRight);
        }

        if (Input::GetMouseButtonDoubleClick(MouseButton::Left))
        {
            Callback(p_mouseOverGo, Action::DoubleClickedLeft);
        }
    }
}

void UIListDriver::AddElement(GameObject *go)
{
    bool hadSelectedGameObject = GetSelectedGameObject();

    go->SetParent(gameObject);

    if (!hadSelectedGameObject) { SetSelection(0); }
}

void UIListDriver::RemoveElement(GameObject *go)
{
    delete go;

    int selIndex = GetSelectedIndex();
    if (p_mouseOverGo == go) { p_mouseOverGo = nullptr; }
    selIndex = Math::Clamp(selIndex, 0, gameObject->GetChildren().Size());
    SetSelection(selIndex);
}

void UIListDriver::Clear()
{
    List<GameObject*> children = gameObject->GetChildren();
    for (GameObject *child : children) { RemoveElement(child); }
}

void UIListDriver::SetSelection(int _i)
{
    const int numChildren = gameObject->GetChildren().Size();
    ENSURE(numChildren > 0);

    int i = ((_i + numChildren) % numChildren);
    GameObject *prevSelectedGo = GetSelectedGameObject();
    if (prevSelectedGo) { Callback(prevSelectedGo, Action::SelectionOut); }

    m_selectionIndex = i;
    GameObject *selectedGo = GetSelectedGameObject();
    if (selectedGo) { Callback(selectedGo, Action::SelectionIn); }
}

void UIListDriver::SetSelection(GameObject *go)
{
    SetSelection(gameObject->GetChildren().IndexOf(go));
}

int UIListDriver::GetSelectedIndex() const
{
    return m_selectionIndex;
}

GameObject *UIListDriver::GetSelectedGameObject() const
{
    return gameObject->GetChild( GetSelectedIndex() );
}

void UIListDriver::SetSelectionCallback(SelectionCallback selectionCallback)
{
    m_selectionCallback = selectionCallback;
}

void UIListDriver::Create()
{
    UIDirLayout *dirLayout;
    const bool vertical = true;
    if (vertical) { dirLayout = gameObject->AddComponent<UIVerticalLayout>(); }
    else { dirLayout = gameObject->AddComponent<UIHorizontalLayout>(); }

    dirLayout->SetSpacing(3);
    dirLayout->SetPaddings(3);

    UIImageRenderer *bg = gameObject->AddComponent<UIImageRenderer>();
    bg->SetTint(Color::White);
}

void UIListDriver::Callback(GameObject *go, Action action)
{
    if (m_selectionCallback) { m_selectionCallback(go, action); }
}
