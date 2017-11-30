#include "Bang/UIButtoneable.h"

#include "Bang/Rect.h"
#include "Bang/Scene.h"
#include "Bang/UICanvas.h"
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

    // Mouse Down event
    if (IsMouseOver())
    {
        if (Input::GetMouseButtonDown(MouseButton::Left))
        {
            m_beingPressed = true;
            for (ClickedCallback callback : m_clickedCallbacks)
            {
                callback(this);
            }
        }
    }

    if (Input::GetMouseButtonUp(MouseButton::Left))
    {
        m_beingPressed = false;
    }
}

bool UIButtoneable::IsBeingPressed() const { return m_beingPressed; }
void UIButtoneable::AddClickedCallback(UIButtoneable::ClickedCallback callback)
{
    m_clickedCallbacks.PushBack(callback);
}
