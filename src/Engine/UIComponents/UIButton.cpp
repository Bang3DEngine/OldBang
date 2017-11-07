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

    // Is mouse currently over some emitter?
    bool mouseOverSomeEmitter = IsMouseOverSomeEmitter();

    // Mouse Down & Up events
    if (mouseOverSomeEmitter)
    {
        Array<MouseButton> buttonsDown = Input::GetMouseButtonsDown();
        for (MouseButton mb : buttonsDown)
        {
            EventEmitter<IUIButtonListener>::
                Propagate(&IUIButtonListener::OnButton_MouseDown, this,
                          SCAST<MouseButton>(mb));
            for (auto f : m_mouseDownCallbacks) { f(this, mb); }
        }

        Array<MouseButton> buttonsUp = Input::GetMouseButtonsUp();
        for (MouseButton mb : buttonsUp)
        {
            EventEmitter<IUIButtonListener>::
                Propagate(&IUIButtonListener::OnButton_MouseUp, this,
                      SCAST<MouseButton>(mb));
            for (auto f : m_mouseUpCallbacks) { f(this, mb); }
        }
    }

    // Clicked event and pressed tracking
    if (mouseOverSomeEmitter)
    {
        m_beingPressed = m_beingPressed ||
                         Input::GetMouseButtonDown(MouseButton::Left);
        if (m_beingPressed && Input::GetMouseButtonUp(MouseButton::Left))
        {
            EventEmitter<IUIButtonListener>::
                Propagate(&IUIButtonListener::OnButton_Clicked, this);
            for (auto f : m_clickedCallbacks) { f(this); }
        }

        if (m_beingPressed &&
            Input::GetMouseButtonDoubleClick(MouseButton::Left))
        {
            EventEmitter<IUIButtonListener>::
                Propagate(&IUIButtonListener::OnButton_DoubleClicked, this);
            for (auto f : m_doubleClickedCallbacks) { f(this); }
        }
    }
    m_beingPressed = ( m_beingPressed &&
                       Input::GetMouseButton(MouseButton::Left) );

    // Mouse Enter & Exit events
    if (!m_mouseOver && mouseOverSomeEmitter)
    {
        EventEmitter<IUIButtonListener>::
            Propagate(&IUIButtonListener::OnButton_MouseEnter, this);
        for (auto f : m_mouseEnterCallbacks) { f(this); }
    }
    else if (m_mouseOver && !mouseOverSomeEmitter)
    {
        EventEmitter<IUIButtonListener>::
            Propagate(&IUIButtonListener::OnButton_MouseExit, this);
        for (auto f : m_mouseExitCallbacks) { f(this); }
    }
    m_mouseOver = mouseOverSomeEmitter;
}

void UIButton::RegisterEmitter(GameObject *emitter)
{
    p_emitters.Add(emitter);
    emitter->RegisterListener(this);
}
void UIButton::UnRegisterEmitter(GameObject *emitter)
{
    p_emitters.Remove(emitter);
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

bool UIButton::IsMouseOverSomeEmitter() const
{
    if (GetMode() == UIButtonMode::UseRender)
    {
        GameObject *overedGameObject = Selection::GetOveredGameObject();
        if (!overedGameObject) { return false; }
        for (auto it = p_emitters.cbegin(); it != p_emitters.cend(); ++it)
        {
            const GameObject *emitter = *it;
            if (overedGameObject == emitter ||
                overedGameObject->IsChildOf(emitter, true))
            {
                return true;
            }
        }
        return false;
    }
    else
    {
        for (auto it = p_emitters.cbegin(); it != p_emitters.cend(); ++it)
        {
           const GameObject *emitter = *it;
           RectTransform *rt = emitter->GetComponent<RectTransform>();
           if (rt && rt->IsMouseOver()) { return true; }
        }
    }
    return false;
}

bool UIButton::IsBeingPressed() const { return m_beingPressed; }
UIButtonMode UIButton::GetMode() const { return m_mode; }

void UIButton::OnBeforeDestroyed(IEventEmitter *destroyedEmitter)
{
    GameObject *destroyedGo = DCAST<GameObject*>(destroyedEmitter);
    if (destroyedGo)
    {
        UnRegisterEmitter(destroyedGo);
    }
}
