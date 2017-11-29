#ifndef UITABBINGGROUP_H
#define UITABBINGGROUP_H

#include "Bang/List.h"
#include "Bang/Alignment.h"
#include "Bang/Component.h"
#include "Bang/IFocusable.h"
#include "Bang/IDestroyListener.h"

NAMESPACE_BANG_BEGIN

class UITabbingGroup : public Component,
                       public IFocusable,
                       public IFocusListener,
                       public IDestroyListener
{
    COMPONENT(UITabbingGroup)

public:
    void OnUpdate() override;

    void AddTabbingChild(IFocusable *focusable);
    void AddTabbingChild(GameObject *gameObject);
    void RemoveTabbingChild(IFocusable *focusable);
    void SetChildFocusIndex(int index);
    void SetChildFocusToBegin();
    void SetChildFocusToEnd();

    void SetParentTabbingGroup(UITabbingGroup *parentTabbingGroup);

    UITabbingGroup* GetParentTabbingGroup() const;
    IFocusable* GetTabbingChildFocusable(int index) const;
    GameObject* GetTabbingChildGameObject(int index) const;
    int GetCurrentTabbingFocusableIndex() const;
    IFocusable* GetCurrentTabbingChildFocusable() const;
    GameObject* GetCurrentTabbingChildGameObject() const;

    const List<IFocusable*>& GetTabbingChildren() const;

    // IDestroyListener
    virtual void OnDestroyed(Object *object) override;

protected:
    // IFocusListener
    void OnFocusTaken(IFocusable *focusable) override;
    void OnFocusLost(IFocusable *focusable) override;

private:
    bool m_hasJustBeenFocused = true;
    int m_currentTabbingFocusableIndex = 0;
    List<IFocusable*> p_tabbingFocusables;
    UITabbingGroup *p_parentTabbingGroup = nullptr;

    UITabbingGroup() = default;
    virtual ~UITabbingGroup() = default;

    int GetClampedIndex(int index) const;

    void GetNearestFocusableChildIndex(int startIndex,
                                       HorizontalSide direction,
                                       int *nearestFocusableIndex,
                                       int *totalDisplacement) const;
    void MoveFocusIndex(int indexIncrement);
    void HandleTabbingGroupChildrenWrapping(UITabbingGroup *tabbingGroupChild,
                                            HorizontalSide wrappingSide);
    void HandleSelfWrapping(int newIndex, HorizontalSide wrapSide);
};

NAMESPACE_BANG_END

#endif // UITABBINGGROUP_H

