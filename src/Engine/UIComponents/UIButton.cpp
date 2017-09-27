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

    // Is mouse currently over some agent?
    bool mouseOverSomeAgent = IsMouseOverSomeAgent();

    // Mouse Down & Up events
    if (mouseOverSomeAgent)
    {
        Array<MouseButton> buttonsDown = Input::GetMouseButtonsDown();
        for (MouseButton mb : buttonsDown)
        {
            PROPAGATE(OnButton_MouseDown(this, mb), p_listeners);
            for (auto f : m_mouseDownCallbacks) { f(this, mb); }
        }

        Array<MouseButton> buttonsUp = Input::GetMouseButtonsUp();
        for (MouseButton mb : buttonsUp)
        {
            PROPAGATE(OnButton_MouseUp(this, mb), p_listeners);
            for (auto f : m_mouseUpCallbacks) { f(this, mb); }
        }
    }

    // Clicked event and pressed tracking
    if (mouseOverSomeAgent)
    {
        m_beingPressed = m_beingPressed ||
                         Input::GetMouseButtonDown(MouseButton::Left);
        if (m_beingPressed && Input::GetMouseButtonUp(MouseButton::Left))
        {
            PROPAGATE(OnButton_Clicked(this), p_listeners);
            for (auto f : m_clickedCallbacks) { f(this); }
        }
    }
    m_beingPressed = ( m_beingPressed &&
                       Input::GetMouseButton(MouseButton::Left) );

    // Mouse Enter & Exit events
    if (!m_mouseOver && mouseOverSomeAgent)
    {
        PROPAGATE(OnButton_MouseEnter(this), p_listeners);
        for (auto f : m_mouseEnterCallbacks) { f(this); }
    }
    else if (m_mouseOver && !mouseOverSomeAgent)
    {
        PROPAGATE(OnButton_MouseExit(this), p_listeners);
        for (auto f : m_mouseExitCallbacks) { f(this); }
    }
    m_mouseOver = mouseOverSomeAgent;
}

void UIButton::AddAgent(GameObject *agent)
{
    p_agents.Add(agent);
}
void UIButton::RemoveAgent(GameObject *agent)
{
    p_agents.Remove(agent);
}

void UIButton::AddListener(UIButtonListener *listener)
{
    p_listeners.Add(listener);
}
void UIButton::RemoveListener(UIButtonListener *listener)
{
    p_listeners.Remove(listener);
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

void UIButton::SetMode(UIButtonMode mode)
{
    m_mode = mode;
}

bool UIButton::IsMouseOverSomeAgent() const
{
    if (GetMode() == UIButtonMode::UseRender)
    {
        GameObject *overedGameObject = Selection::GetOveredGameObject();
        if (!overedGameObject) { return false; }
        for (auto it = p_agents.cbegin(); it != p_agents.cend(); ++it)
        {
            const GameObject *agent = *it;
            if (overedGameObject == agent ||
                overedGameObject->IsChildOf(agent, true))
            {
                return true;
            }
        }
        return false;
    }
    else
    {
        Vector2 mouseCoordsNDC = Input::GetMouseCoordsNDC();
        for (auto it = p_agents.cbegin(); it != p_agents.cend(); ++it)
        {
           const GameObject *agent = *it;
           RectTransform *rt = agent->GetComponent<RectTransform>();
           if (rt && rt->GetScreenSpaceRectNDC().Contains(mouseCoordsNDC))
           {
               return true;
           }
        }
    }
    return false;
}

bool UIButton::IsBeingPressed() const
{
    return m_beingPressed;
}

UIButtonMode UIButton::GetMode() const
{
    return m_mode;
}
