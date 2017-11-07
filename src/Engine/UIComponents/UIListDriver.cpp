#include "Bang/UIListDriver.h"

#include "Bang/Rect.h"
#include "Bang/Input.h"
#include "Bang/GameObject.h"
#include "Bang/UIScrollBar.h"
#include "Bang/UIScrollArea.h"
#include "Bang/UIScrollPanel.h"
#include "Bang/RectTransform.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UILayoutIgnorer.h"
#include "Bang/UIVerticalLayout.h"
#include "Bang/GameObjectFactory.h"
#include "Bang/UIHorizontalLayout.h"
#include "Bang/UIContentSizeFitter.h"

USING_NAMESPACE_BANG

UIListDriver::UIListDriver()
{
}

UIListDriver::~UIListDriver()
{
}

void UIListDriver::OnUpdate()
{
    if (GetGameObject()->GetComponent<RectTransform>()->IsMouseOver())
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
    GameObject::Destroy(go);

    int selIndex = GetSelectedIndex();
    if (p_mouseOverGo == go) { p_mouseOverGo = nullptr; }
    selIndex = Math::Clamp(selIndex, 0, GetContainer()->GetChildren().Size());
    SetSelection(selIndex);
}

void UIListDriver::Clear()
{
    List<GameObject*> children = GetContainer()->GetChildren();
    for (GameObject *child : children) { RemoveElement(child); }
    GetScrollPanel()->SetScrollingPercent( Vector2(0.0f) );
}

void UIListDriver::SetSelection(int _i)
{
    const int numChildren = GetContainer()->GetChildren().Size();
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
    SetSelection(GetContainer()->GetChildren().IndexOf(go));
}

GameObject *UIListDriver::GetContainer() const
{
    return p_container;
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
    REQUIRE_COMPONENT(go, RectTransform);

    UIScrollPanel *scrollPanel = GameObjectFactory::CreateUIScrollPanelInto(go);

    UILayoutElement *le = go->AddComponent<UILayoutElement>();
    le->SetMinHeight(10);

    GameObject *container = GameObjectFactory::CreateUIGameObject();

    UIDirLayout *dirLayout;
    const bool vertical = true;
    if (vertical) { dirLayout = container->AddComponent<UIVerticalLayout>(); }
    else { dirLayout = container->AddComponent<UIHorizontalLayout>(); }
    dirLayout->SetSpacing(0);
    dirLayout->SetPaddings(0);

    container->GetComponent<RectTransform>()->SetPivotPosition(Vector2(-1,1));
    UIContentSizeFitter *csf = container->AddComponent<UIContentSizeFitter>();
    csf->SetHorizontalSizeType(LayoutSizeType::Preferred);
    csf->SetVerticalSizeType(LayoutSizeType::Preferred);

    scrollPanel->GetScrollArea()->SetContainedGameObject(container);

    UIListDriver *ld = go->AddComponent<UIListDriver>();
    ld->p_scrollPanel = scrollPanel;
    ld->p_container = container;

    return ld;
}

void UIListDriver::Callback(GameObject *go, Action action)
{
    if (m_selectionCallback) { m_selectionCallback(go, action); }
}

UIScrollPanel *UIListDriver::GetScrollPanel() const { return p_scrollPanel; }
