#include "Bang/UITabbingGroup.h"

#include "Bang/Input.h"
#include "Bang/UICanvas.h"
#include "Bang/GameObject.h"

USING_NAMESPACE_BANG

void UITabbingGroup::OnUpdate()
{
    Component::OnUpdate();

    if (UICanvas::HasFocus(this))
    {
        if (Input::GetKeyDownRepeat(Key::Tab))
        {
            int inc = (Input::GetKey(Key::LShift) ||
                       Input::GetKey(Key::RShift)) ? -1 : 1;

            if (!GetTabbingFocusables().IsEmpty())
            {
                const int n = GetTabbingFocusables().Size();
                const int prevIndex = GetCurrentTabbingFocusableIndex();
                while (true)
                {
                    m_currentTabbingFocusableIndex =
                            (GetCurrentTabbingFocusableIndex() + inc + n) % n;

                    if (GetCurrentTabbingFocusableIndex() == prevIndex) { break; }

                    const bool isValid =
                        (GetCurrentTabbingFocusable()->IsFocusEnabled() &&
                         GetCurrentTabbingGameObject()->IsEnabled());
                    if (isValid) { break; }
                }

                Debug_Log(this << ": " << m_currentTabbingFocusableIndex);
                UICanvas::SetFocus( GetCurrentTabbingFocusable() );
            }
        }
    }
}

int UITabbingGroup::GetCurrentTabbingFocusableIndex() const
{
    return m_currentTabbingFocusableIndex;
}

IFocusable *UITabbingGroup::GetCurrentTabbingFocusable() const
{
    auto it = GetTabbingFocusables().Begin();
    std::advance(it, GetCurrentTabbingFocusableIndex());
    return *it;
}

GameObject *UITabbingGroup::GetCurrentTabbingGameObject() const
{
    IFocusable *focusable = GetCurrentTabbingFocusable();
    return focusable ? Cast<Component*>(focusable)->GetGameObject() : nullptr;
}

void UITabbingGroup::AddTabbingFocusable(IFocusable *focusable)
{
    Component *focusableComp = Cast<Component*>(focusable);
    focusableComp->EventEmitter<IDestroyListener>::RegisterListener(this);
    p_tabbingFocusables.PushBack(focusable);
}

void UITabbingGroup::AddTabbingFocusable(GameObject *gameObject)
{
    IFocusable *focusable = gameObject->GetComponent<IFocusable>();
    ASSERT(focusable);
    AddTabbingFocusable(focusable);
}

void UITabbingGroup::AddTabbingFocusable(UITabbingGroup *tabbingGroup)
{

}

const List<IFocusable*>& UITabbingGroup::GetTabbingFocusables() const
{
    return p_tabbingFocusables;
}

void UITabbingGroup::OnDestroyed(Object *object)
{
    p_tabbingFocusables.Remove( Cast<IFocusable*>(object) );
}

void UITabbingGroup::OnFocusTaken()
{
    IFocusListener::OnFocusTaken();
    if (!GetTabbingFocusables().IsEmpty())
    {
        UICanvas::SetFocus( GetTabbingFocusables().Front() );
    }
}

void UITabbingGroup::OnFocusLost()
{
    IFocusListener::OnFocusLost();
}

UITabbingGroup::UITabbingGroup()
{
}

UITabbingGroup::~UITabbingGroup()
{
}

