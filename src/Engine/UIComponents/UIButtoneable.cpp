#include "Bang/UIButtoneable.h"

#include "Bang/Rect.h"
#include "Bang/Scene.h"
#include "Bang/Selection.h"
#include "Bang/GameObject.h"
#include "Bang/RectTransform.h"

USING_NAMESPACE_BANG

UIButtoneable::UIButtoneable() : Component()
{

}

UIButtoneable::~UIButtoneable()
{

}

void UIButtoneable::OnUpdate()
{
    Component::OnUpdate();

    // Is mouse currently over some button part?
    const bool mouseOverSomePart = IsMouseOverSomePart();

    // Mouse Down event
    if (mouseOverSomePart)
    {
        Array<MouseButton> buttonsDown = Input::GetMouseButtonsDown();
        for (MouseButton mb : buttonsDown)
        {
            PROPAGATE(IUIButtonListener, OnButton_MouseDown, this, mb);
            for (auto f : m_mouseDownCallbacks) { f(this, mb); }
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

    // Mouse Up event
    Array<MouseButton> buttonsUp = Input::GetMouseButtonsUp();
    for (MouseButton mb : buttonsUp)
    {
        PROPAGATE(IUIButtonListener, OnButton_MouseUp, this, mb, mouseOverSomePart);
        for (auto f : m_mouseUpCallbacks) { f(this, mb, mouseOverSomePart); }
    }

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

void UIButtoneable::RegisterButtonPart(GameObject *buttonPart)
{
    p_buttonParts.Add(buttonPart);
    buttonPart->EventEmitter<IDestroyListener>::RegisterListener(this);
}
void UIButtoneable::UnRegisterButtonPart(GameObject *buttonPart)
{
    p_buttonParts.Remove(buttonPart);
}

void UIButtoneable::SetMode(UIButtoneableMode mode)
{
    m_mode = mode;
}

bool UIButtoneable::IsMouseOverSomePart() const
{
    if (GetMode() == UIButtoneableMode::Render)
    {
        GameObject *overedGameObject =
                  Selection::GetOveredGameObject( GetGameObject()->GetScene() );
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

bool UIButtoneable::IsBeingPressed() const { return m_beingPressed; }
UIButtoneableMode UIButtoneable::GetMode() const { return m_mode; }

void UIButtoneable::OnDestroyed(Object *object)
{
    GameObject *destroyedGo = DCAST<GameObject*>(object);
    if (destroyedGo)
    {
        UnRegisterButtonPart(destroyedGo);
    }
}

void UIButtoneable::AddMouseEnterCallback(UIButtoneable::EnterExitCallback callback)
{
    m_mouseEnterCallbacks.PushBack(callback);
}
void UIButtoneable::AddMouseExitCallback(UIButtoneable::EnterExitCallback callback)
{
    m_mouseExitCallbacks.PushBack(callback);
}
void UIButtoneable::AddMouseDownCallback(UIButtoneable::DownCallback callback)
{
    m_mouseDownCallbacks.PushBack(callback);
}
void UIButtoneable::AddMouseUpCallback(UIButtoneable::UpCallback callback)
{
    m_mouseUpCallbacks.PushBack(callback);
}
void UIButtoneable::AddClickedCallback(UIButtoneable::ClickedCallback callback)
{
    m_clickedCallbacks.PushBack(callback);
}

void UIButtoneable::AddDoubleClickedCallback(UIButtoneable::DoubleClickedCallback callback)
{
    m_doubleClickedCallbacks.PushBack(callback);
}

