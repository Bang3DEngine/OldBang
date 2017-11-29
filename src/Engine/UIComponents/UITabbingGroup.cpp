#include "Bang/UITabbingGroup.h"

#include "Bang/Input.h"
#include "Bang/UICanvas.h"
#include "Bang/GameObject.h"

USING_NAMESPACE_BANG

void UITabbingGroup::OnUpdate()
{
    Component::OnUpdate();

    if (UICanvas::HasFocus(this) && !m_hasJustBeenFocused)
    {
        if (Input::GetKeyDownRepeat(Key::Tab))
        {
            IFocusable *focusableChild = GetCurrentTabbingChildFocusable();
            bool hasChildTabbingGroupFocused =
                    focusableChild && Cast<UITabbingGroup*>(focusableChild);

            if (!hasChildTabbingGroupFocused)
            {

                if (!GetTabbingChildren().IsEmpty())
                {
                    bool shift = (Input::GetKey(Key::LShift) ||
                                          Input::GetKey(Key::RShift));
                    HorizontalSide moveDir = (shift ? HorizontalSide::Left :
                                                      HorizontalSide::Right);
                    const int startIndex = GetCurrentTabbingFocusableIndex();

                    int totalIncrement;
                    GetNearestFocusableChildIndex(startIndex, moveDir,
                                                  nullptr, &totalIncrement);
                    MoveFocusIndex(totalIncrement); // Move the focus index
                }
            }
        }
    }
    m_hasJustBeenFocused = false;
}

void UITabbingGroup::AddTabbingChild(IFocusable *focusable)
{
    focusable->EventEmitter<IFocusListener>::RegisterListener(this);

    Component *focusableComp = Cast<Component*>(focusable);
    focusableComp->EventEmitter<IDestroyListener>::RegisterListener(this);

    p_tabbingFocusables.PushBack(focusable);
}

void UITabbingGroup::AddTabbingChild(GameObject *gameObject)
{
    IFocusable *focusable = gameObject->GetComponent<IFocusable>();
    ASSERT(focusable);
    AddTabbingChild(focusable);
}

void UITabbingGroup::RemoveTabbingChild(IFocusable *focusable)
{
    p_tabbingFocusables.Remove(focusable);
}

void UITabbingGroup::SetChildFocusIndex(int index)
{
    m_currentTabbingFocusableIndex = index;
    UICanvas::SetFocus( GetCurrentTabbingChildFocusable() );
}
void UITabbingGroup::MoveFocusIndex(int indexIncrement)
{
    // Moves the focus index the amount specified by indexIncrement
    const int prevIndex = GetCurrentTabbingFocusableIndex();
    const int newIndex = (prevIndex + indexIncrement);

    bool hasWrapped = (newIndex < 0 || newIndex >= GetTabbingChildren().Size());
    if (!hasWrapped)
    {
        // Set the index normally
        SetChildFocusIndex( newIndex );
        UITabbingGroup *tabbingGroup =
                    Cast<UITabbingGroup*>(GetCurrentTabbingChildFocusable());
        if (tabbingGroup)
        {
            if (indexIncrement >= 0) { tabbingGroup->SetChildFocusToBegin(); }
            else { tabbingGroup->SetChildFocusToEnd(); }
        }
    }
    else
    {
        // We have wrapped! Call the function that handles this...
        HorizontalSide wrapSide =
          (indexIncrement >= 0 ? HorizontalSide::Right : HorizontalSide::Left);
        HandleSelfWrapping(GetClampedIndex(newIndex), wrapSide);
    }
}

void UITabbingGroup::SetChildFocusToBegin()
{
    if (!GetTabbingChildren().IsEmpty())
    {
        int nearestIndexToBegin;
        GetNearestFocusableChildIndex(-1, HorizontalSide::Right,
                                      &nearestIndexToBegin, nullptr);
        SetChildFocusIndex(nearestIndexToBegin);
    }
}

void UITabbingGroup::SetChildFocusToEnd()
{
    if (!GetTabbingChildren().IsEmpty())
    {
        int nearestIndexToEnd;
        GetNearestFocusableChildIndex(GetTabbingChildren().Size(),
                                      HorizontalSide::Left,
                                      &nearestIndexToEnd, nullptr);
        SetChildFocusIndex(nearestIndexToEnd);
    }
}

IFocusable *UITabbingGroup::GetTabbingChildFocusable(int index) const
{
    if (index < 0 || index >= GetTabbingChildren().Size())
    {
        return nullptr;
    }

    auto it = GetTabbingChildren().Begin();
    std::advance(it, index);
    return *it;
}

GameObject *UITabbingGroup::GetTabbingChildGameObject(int index) const
{
    IFocusable *focusable = GetTabbingChildFocusable(index);
    return focusable ? Cast<Component*>(focusable)->GetGameObject() : nullptr;
}
int UITabbingGroup::GetCurrentTabbingFocusableIndex() const
{
    return m_currentTabbingFocusableIndex;
}

IFocusable *UITabbingGroup::GetCurrentTabbingChildFocusable() const
{
    return GetTabbingChildFocusable( GetCurrentTabbingFocusableIndex() );
}

GameObject *UITabbingGroup::GetCurrentTabbingChildGameObject() const
{
    return GetTabbingChildGameObject( GetCurrentTabbingFocusableIndex() );
}

const List<IFocusable*>& UITabbingGroup::GetTabbingChildren() const
{
    return p_tabbingFocusables;
}

void UITabbingGroup::OnDestroyed(Object *object)
{
    p_tabbingFocusables.Remove( Cast<IFocusable*>(object) );
}

void UITabbingGroup::OnFocusTaken(IFocusable *focusable)
{
    IFocusListener::OnFocusTaken(focusable);

    if (focusable == this)
    {
        if (!GetTabbingChildren().IsEmpty())
        {
            UICanvas::SetFocus( GetTabbingChildren().Front() );
        }
    }
    else // A child has been focused
    {
        int tabbingGroupChildIndex = GetTabbingChildren().IndexOf(focusable);
        ASSERT(tabbingGroupChildIndex >= 0);

        SetChildFocusIndex(tabbingGroupChildIndex);
    }
    m_hasJustBeenFocused = true;
}

void UITabbingGroup::OnFocusLost(IFocusable *focusable)
{
    IFocusListener::OnFocusLost(focusable);
}

int UITabbingGroup::GetClampedIndex(int index) const
{
    int n = GetTabbingChildren().Size();
    if (n == 0) { return 0; }
    return (index + n) % n;
}

void UITabbingGroup::GetNearestFocusableChildIndex(int startIndex,
                                                   HorizontalSide direction,
                                                   int *nearestFocusableIndex,
                                                   int *totalDisplacement) const
{
    int _totalDisplacement = 0;
    int _nearestFocusableIndex;
    int increment = (direction == HorizontalSide::Right ? 1 : -1);
    while (true)
    {
        _totalDisplacement += increment;
        _nearestFocusableIndex = GetClampedIndex(startIndex + _totalDisplacement);
        if (_nearestFocusableIndex == startIndex) { break; }

        const bool isValid =
            (GetTabbingChildFocusable(_nearestFocusableIndex)->IsFocusEnabled() &&
             GetTabbingChildGameObject(_nearestFocusableIndex)->IsEnabled());
        if (isValid) { break; }
    }

    if (nearestFocusableIndex) { *nearestFocusableIndex = _nearestFocusableIndex; }
    if (totalDisplacement) { *totalDisplacement = _totalDisplacement; }
}

void UITabbingGroup::HandleTabbingGroupChildrenWrapping(
                                        UITabbingGroup *tabbingGroupChild,
                                        HorizontalSide wrappingSide)
{
    // A child tabbing group has wrapped (left or right). Let's handle that!
    int tabbingGroupChildIndex = GetTabbingChildren().IndexOf(tabbingGroupChild);
    ASSERT(tabbingGroupChildIndex >= 0);

    MoveFocusIndex( (wrappingSide == HorizontalSide::Right ? 1 : -1) );

    // In case we are moving to a child which is UITabbingGroup, depending on
    // the wrapping side, we want it to focus either to its begin or end.
    IFocusable *newFocused = GetCurrentTabbingChildFocusable();
    UITabbingGroup *newFocusableTabbingGroup = Cast<UITabbingGroup*>(newFocused);
    if (newFocusableTabbingGroup)
    {
        if (wrappingSide == HorizontalSide::Right)
        {
            newFocusableTabbingGroup->SetChildFocusToBegin();
        }
        else // Left
        {
            newFocusableTabbingGroup->SetChildFocusToEnd();
        }
    }
}

void UITabbingGroup::HandleSelfWrapping(int newIndex, HorizontalSide wrapSide)
{
    ASSERT(newIndex >= 0 && newIndex < GetTabbingChildren().Size());
    if (!GetParentTabbingGroup())
    {
        // If we dont have a parent tabbing group, just wrap normally
        SetChildFocusIndex(newIndex);
    }
    else
    {
        // Ask parent what to do on wrap (do wrap, or do nothing because he will
        // handle the wrapping (because having other UITabbingGroup siblings) )
        GetParentTabbingGroup()->HandleTabbingGroupChildrenWrapping(this, wrapSide);
    }
}

void UITabbingGroup::SetParentTabbingGroup(UITabbingGroup *parentTabbingGroup)
{
    if (GetParentTabbingGroup())
    {
        GetParentTabbingGroup()->RemoveTabbingChild(this);
    }

    p_parentTabbingGroup = parentTabbingGroup;
    if (GetParentTabbingGroup())
    {
        GetParentTabbingGroup()->AddTabbingChild(this);
    }
}

UITabbingGroup* UITabbingGroup::GetParentTabbingGroup() const
{
    return p_parentTabbingGroup;
}

