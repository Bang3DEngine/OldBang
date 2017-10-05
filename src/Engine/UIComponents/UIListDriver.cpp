#include "Bang/UIListDriver.h"

#include "Bang/Rect.h"
#include "Bang/Input.h"
#include "Bang/GameObject.h"
#include "Bang/UIScrollArea.h"
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
    UIFocusTaker *ft = GetContainer()->GetComponent<UIFocusTaker>();
    ft->SetDefaultFocusAction(FocusAction::TakeIt);

    if (ft->HasFocus())
    {
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
        for (GameObject *child : GetContainer()->GetChildren())
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
}

void UIListDriver::AddElement(GameObject *go)
{
    bool hadSelectedGameObject = GetSelectedGameObject();

    go->SetParent(GetContainer());

    if (!hadSelectedGameObject) { SetSelection(0); }
}

void UIListDriver::RemoveElement(GameObject *go)
{
    delete go;

    int selIndex = GetSelectedIndex();
    if (p_mouseOverGo == go) { p_mouseOverGo = nullptr; }
    selIndex = Math::Clamp(selIndex, 0, GetContainer()->GetChildren().Size());
    SetSelection(selIndex);
}

void UIListDriver::Clear()
{
    List<GameObject*> children = GetContainer()->GetChildren();
    for (GameObject *child : children) { RemoveElement(child); }
}

void UIListDriver::SetSelection(int _i)
{
    const int numChildren = GetContainer()->GetChildren().Size();
    ENSURE(numChildren > 0);

    p_scrollArea->SetScrollingY( p_scrollArea->GetScrolling().y + 1 );

    int i = ((_i + numChildren) % numChildren);
    GameObject *prevSelectedGo = GetSelectedGameObject();
    if (prevSelectedGo) { Callback(prevSelectedGo, Action::SelectionOut); }

    m_selectionIndex = i;
    GameObject *selectedGo = GetSelectedGameObject();
    if (selectedGo) { Callback(selectedGo, Action::SelectionIn); }
}

void UIListDriver::SetSelection(GameObject *go)
{
    SetSelection(GetContainer()->GetChildren().IndexOf(go));
}

GameObject *UIListDriver::GetContainer() const
{
    return p_scrollArea->GetContainer();
}

int UIListDriver::GetSelectedIndex() const
{
    return m_selectionIndex;
}

GameObject *UIListDriver::GetSelectedGameObject() const
{
    return GetContainer()->GetChild( GetSelectedIndex() );
}

void UIListDriver::SetSelectionCallback(SelectionCallback selectionCallback)
{
    m_selectionCallback = selectionCallback;
}

UIListDriver* UIListDriver::CreateInto(GameObject *go)
{
    UIListDriver *ld = go->AddComponent<UIListDriver>();

    ld->p_scrollArea = GameObjectFactory::CreateGUIScrollAreaInto(go);
    GameObject *container = ld->p_scrollArea->GetContainer();

    UIDirLayout *dirLayout;
    const bool vertical = true;
    if (vertical) { dirLayout = container->AddComponent<UIVerticalLayout>(); }
    else { dirLayout = container->AddComponent<UIHorizontalLayout>(); }

    dirLayout->SetSpacing(3);
    dirLayout->SetPaddings(3);

    UIImageRenderer *bg = container->AddComponent<UIImageRenderer>();
    bg->SetTint(Color::White);

    Debug_Peek(ld->p_scrollArea);
    Debug_Peek(ld->p_scrollArea->gameObject);
    Debug_Peek(ld->p_scrollArea->GetContainer());
    return ld;
}

void UIListDriver::Callback(GameObject *go, Action action)
{
    if (m_selectionCallback) { m_selectionCallback(go, action); }
}
