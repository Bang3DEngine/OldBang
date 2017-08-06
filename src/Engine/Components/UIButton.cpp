#include "Bang/UIButton.h"

#include "Bang/Selection.h"
#include "Bang/GameObject.h"

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
    bool mouseOverSomeAgent = false;
    GameObject *overedGameObject = Selection::GetOveredGameObject();
    for (GameObject *agent : p_agents)
    {
        if (overedGameObject == agent)
        {
            mouseOverSomeAgent = true;
            break;
        }
    }

    // Mouse Down & Up events
    if (mouseOverSomeAgent)
    {
        Array<Input::MouseButton> buttonsDown = Input::GetMouseButtonsDown();
        for (Input::MouseButton mb : buttonsDown)
        {
            PROPAGATE_EVENT_RAW(OnButton_MouseDown(this, mb), p_listeners);
            for (auto f : m_mouseDownCallbacks) { f(this, mb); }
        }

        Array<Input::MouseButton> buttonsUp = Input::GetMouseButtonsUp();
        for (Input::MouseButton mb : buttonsUp)
        {
            PROPAGATE_EVENT_RAW(OnButton_MouseUp(this, mb), p_listeners);
            for (auto f : m_mouseUpCallbacks) { f(this, mb); }
        }
    }

    // Mouse Enter & Exit events
    if (!m_mouseOver && mouseOverSomeAgent)
    {
        PROPAGATE_EVENT_RAW(OnButton_MouseEnter(this), p_listeners);
        for (auto f : m_mouseEnterCallbacks) { f(this); }
    }
    else if (m_mouseOver && !mouseOverSomeAgent)
    {
        PROPAGATE_EVENT_RAW(OnButton_MouseExit(this), p_listeners);
        for (auto f : m_mouseExitCallbacks) { f(this); }
    }
    m_mouseOver = mouseOverSomeAgent;
}

void UIButton::AddAgent(GameObject *agent)
{
    List<GameObject*> recChildren = agent->GetChildrenRecursively();
    for (GameObject *rChild : recChildren)
    {
        p_agents.Add(rChild);
    }
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
