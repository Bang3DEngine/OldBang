#include "Bang/UIButton.h"

#include "Bang/Rect.h"
#include "Bang/Selection.h"
#include "Bang/GameObject.h"
#include "Bang/RectTransform.h"

USING_NAMESPACE_BANG

UIButton::UIButton() : Component()
{

}

UIButton::~UIButton()
{

}

void UIButton::OnUpdate()
{
    Component::OnUpdate();

    // Is mouse currently over some button part?
    const bool mouseOverSomePart = IsMouseOverSomePart();

    // Mouse Down & Up events
    if (mouseOverSomePart)
    {
        Array<MouseButton> buttonsDown = Input::GetMouseButtonsDown();
        for (MouseButton mb : buttonsDown)
        {
            PROPAGATE(IUIButtonListener, OnButton_MouseDown, this, mb);
            for (auto f : m_mouseDownCallbacks) { f(this, mb); }
        }

        Array<MouseButton> buttonsUp = Input::GetMouseButtonsUp();
        for (MouseButton mb : buttonsUp)
        {
            PROPAGATE(IUIButtonListener, OnButton_MouseUp, this, mb);
            for (auto f : m_mouseUpCallbacks) { f(this, mb); }
        }
    }

    // Clicked event and pressed tracking
    if (mouseOverSomePart)
    {
        m_beingPressed = m_beingPressed ||
                         Input::GetMouseButtonDown(MouseButton::Left);
        if (m_beingPressed && Input::GetMouseButtonUp(MouseButton::Left))
        {
            PROPAGATE(IUIButtonListener, OnButton_Clicked, this);
            for (auto f : m_clickedCallbacks) { f(this); }
        }

        if (m_beingPressed &&
            Input::GetMouseButtonDoubleClick(MouseButton::Left))
        {
            PROPAGATE(IUIButtonListener, OnButton_DoubleClicked, this);
            for (auto f : m_doubleClickedCallbacks) { f(this); }
        }
    }
    m_beingPressed = ( m_beingPressed &&
                       Input::GetMouseButton(MouseButton::Left) );

    // Mouse Enter & Exit events
    if (!m_mouseOver && mouseOverSomePart)
    {
        PROPAGATE(IUIButtonListener, OnButton_MouseEnter, this);
        for (auto f : m_mouseEnterCallbacks) { f(this); }
    }
    else if (m_mouseOver && !mouseOverSomePart)
    {
        PROPAGATE(IUIButtonListener, OnButton_MouseExit, this);
        for (auto f : m_mouseExitCallbacks) { f(this); }
    }
    m_mouseOver = mouseOverSomePart;
}

void UIButton::RegisterButtonPart(GameObject *buttonPart)
{
    p_buttonParts.Add(buttonPart);
    buttonPart->EventEmitter<IDestroyListener>::RegisterListener(this);
}
void UIButton::UnRegisterButtonPart(GameObject *buttonPart)
{
    p_buttonParts.Remove(buttonPart);
}

void UIButton::AddMouseEnterCallback(UIButton::EnterExitCallback callback)
{
    m_mouseEnterCallbacks.PushBack(callback);
}
void UIButton::AddMouseExitCallback(UIButton::EnterExitCallback callback)
{
    m_mouseExitCallbacks.PushBack(callback);
}
void UIButton::AddMouseDownCallback(UIButton::DownUpCallback callback)
{
    m_mouseDownCallbacks.PushBack(callback);
}
void UIButton::AddMouseUpCallback(UIButton::DownUpCallback callback)
{
    m_mouseUpCallbacks.PushBack(callback);
}
void UIButton::AddClickedCallback(UIButton::ClickedCallback callback)
{
    m_clickedCallbacks.PushBack(callback);
}

void UIButton::AddDoubleClickedCallback(UIButton::DoubleClickedCallback callback)
{
    m_doubleClickedCallbacks.PushBack(callback);
}

void UIButton::SetMode(UIButtonMode mode)
{
    m_mode = mode;
}

bool UIButton::IsMouseOverSomePart() const
{
    if (GetMode() == UIButtonMode::UseRender)
    {
        GameObject *overedGameObject = Selection::GetOveredGameObject();
        if (!overedGameObject) { return false; }
        for (const GameObject *part : p_buttonParts)
        {
            if (!part->IsEnabled(true)) { continue; }
            if (overedGameObject == part) { return true; }
        }
    }
    else
    {
        for (const GameObject *part : p_buttonParts)
        {
           if (!part->IsEnabled(true)) { continue; }
           RectTransform *rt = part->GetComponent<RectTransform>();
           if (rt && rt->IsMouseOver()) { return true; }
        }
    }
    return false;
}

bool UIButton::IsBeingPressed() const { return m_beingPressed; }
UIButtonMode UIButton::GetMode() const { return m_mode; }

void UIButton::OnBeforeDestroyed(Object *object)
{
    GameObject *destroyedGo = DCAST<GameObject*>(object);
    if (destroyedGo)
    {
        UnRegisterButtonPart(destroyedGo);
    }
}
