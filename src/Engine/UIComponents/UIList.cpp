#include "Bang/UIList.h"

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

UIList::UIList()
{
}

UIList::~UIList()
{
}

void UIList::OnUpdate()
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
        GameObject *goUnderMouse = nullptr;
        for (GameObject *child : GetContainer()->GetChildren())
        {
            RectTransform *rt = child->GetComponent<RectTransform>();
            if (rt && rt->IsMouseOver()) { goUnderMouse = child; break; }
        }

        if (p_mouseOverGo != goUnderMouse)
        {
            if (p_mouseOverGo) { Callback(p_mouseOverGo, Action::MouseOut); }

            p_mouseOverGo = goUnderMouse;
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
    else
    {
        if (p_mouseOverGo) { Callback(p_mouseOverGo, Action::MouseOut); }
        p_mouseOverGo = nullptr;
    }
}

void UIList::AddElement(GameObject *go)
{
    bool hadSelectedGameObject = GetSelectedGameObject();

    go->SetParent(GetContainer());

    if (!hadSelectedGameObject) { SetSelection(0); }
}

void UIList::RemoveElement(GameObject *go)
{
    GameObject::Destroy(go);

    int selIndex = GetSelectedIndex();
    if (p_mouseOverGo == go) { p_mouseOverGo = nullptr; }
    selIndex = Math::Clamp(selIndex, 0, GetContainer()->GetChildren().Size());
    SetSelection(selIndex);
}

void UIList::Clear()
{
    List<GameObject*> children = GetContainer()->GetChildren();
    for (GameObject *child : children) { RemoveElement(child); }
    GetScrollPanel()->SetScrollingPercent( Vector2(0.0f) );
}

void UIList::SetSelection(int _i)
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

void UIList::SetSelection(GameObject *go)
{
    SetSelection(GetContainer()->GetChildren().IndexOf(go));
}

GameObject *UIList::GetContainer() const
{
    return p_container;
}

int UIList::GetSelectedIndex() const
{
    return m_selectionIndex;
}

GameObject *UIList::GetSelectedGameObject() const
{
    return GetContainer()->GetChild( GetSelectedIndex() );
}

void UIList::SetSelectionCallback(SelectionCallback selectionCallback)
{
    m_selectionCallback = selectionCallback;
}

UIList* UIList::CreateInto(GameObject *go)
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

    UIList *ld = go->AddComponent<UIList>();
    ld->p_scrollPanel = scrollPanel;
    ld->p_container = container;

    return ld;
}

void UIList::Callback(GameObject *go, Action action)
{
    if (m_selectionCallback) { m_selectionCallback(go, action); }
}

UIScrollPanel *UIList::GetScrollPanel() const { return p_scrollPanel; }
