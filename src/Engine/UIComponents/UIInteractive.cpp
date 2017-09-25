#include "Bang/UIInteractive.h"

#include "Bang/Rect.h"
#include "Bang/Selection.h"
#include "Bang/GameObject.h"
#include "Bang/RectTransform.h"

USING_NAMESPACE_BANG

UIInteractive::UIInteractive() : Component()
{

}

UIInteractive::~UIInteractive()
{

}

void UIInteractive::OnUpdate()
{
    Component::OnUpdate();

    // Is mouse currently over some agent?
    bool mouseOverSomeAgent = IsMouseOverSomeAgent();

    // Mouse Down & Up events
    if (mouseOverSomeAgent)
    {
        Array<Input::MouseButton> buttonsDown = Input::GetMouseButtonsDown();
        for (Input::MouseButton mb : buttonsDown)
        {
            PROPAGATE(OnButton_MouseDown(this, mb), p_listeners);
            for (auto f : m_mouseDownCallbacks) { f(this, mb); }
        }

        Array<Input::MouseButton> buttonsUp = Input::GetMouseButtonsUp();
        for (Input::MouseButton mb : buttonsUp)
        {
            PROPAGATE(OnButton_MouseUp(this, mb), p_listeners);
            for (auto f : m_mouseUpCallbacks) { f(this, mb); }
        }
    }

    // Clicked event and pressed tracking
    if (mouseOverSomeAgent)
    {
        m_beingPressed = m_beingPressed ||
                         Input::GetMouseButtonDown(Input::MouseButton::Left);
        if (m_beingPressed && Input::GetMouseButtonUp(Input::MouseButton::Left))
        {
            PROPAGATE(OnButton_Clicked(this), p_listeners);
            for (auto f : m_clickedCallbacks) { f(this); }
        }
    }
    m_beingPressed = ( m_beingPressed &&
                       Input::GetMouseButton(Input::MouseButton::Left) );

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

void UIInteractive::AddAgent(GameObject *agent)
{
    p_agents.Add(agent);
}
void UIInteractive::RemoveAgent(GameObject *agent)
{
    p_agents.Remove(agent);
}

void UIInteractive::AddListener(UIInteractiveListener *listener)
{
    p_listeners.Add(listener);
}
void UIInteractive::RemoveListener(UIInteractiveListener *listener)
{
    p_listeners.Remove(listener);
}

void UIInteractive::AddMouseEnterCallback(UIInteractive::EnterExitCallback callback)
{
    m_mouseEnterCallbacks.PushBack(callback);
}
void UIInteractive::AddMouseExitCallback(UIInteractive::EnterExitCallback callback)
{
    m_mouseExitCallbacks.PushBack(callback);
}
void UIInteractive::AddMouseDownCallback(UIInteractive::DownUpCallback callback)
{
    m_mouseDownCallbacks.PushBack(callback);
}
void UIInteractive::AddMouseUpCallback(UIInteractive::DownUpCallback callback)
{
    m_mouseUpCallbacks.PushBack(callback);
}
void UIInteractive::AddClickedCallback(UIInteractive::ClickedCallback callback)
{
    m_clickedCallbacks.PushBack(callback);
}

void UIInteractive::SetMode(UIInteractiveMode mode)
{
    m_mode = mode;
}

bool UIInteractive::IsMouseOverSomeAgent() const
{
    if (GetMode() == UIInteractiveMode::UseRender)
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

bool UIInteractive::IsBeingPressed() const
{
    return m_beingPressed;
}

UIInteractiveMode UIInteractive::GetMode() const
{
    return m_mode;
}
