#ifndef UITABBINGGROUP_H
#define UITABBINGGROUP_H

#include "Bang/List.h"
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

    void AddTabbingFocusable(IFocusable *focusable);
    void AddTabbingFocusable(GameObject *gameObject);
    void AddTabbingFocusable(UITabbingGroup *tabbingGroup);

    int GetCurrentTabbingFocusableIndex() const;
    IFocusable* GetCurrentTabbingFocusable() const;
    GameObject* GetCurrentTabbingGameObject() const;

    const List<IFocusable*>& GetTabbingFocusables() const;

    // IDestroyListener
    virtual void OnDestroyed(Object *object) override;

protected:
    // IFocusListener
    void OnFocusTaken() override;
    void OnFocusLost() override;

private:
    int m_currentTabbingFocusableIndex = 0;
    List<IFocusable*> p_tabbingFocusables;

	UITabbingGroup();
	virtual ~UITabbingGroup();
};

NAMESPACE_BANG_END

#endif // UITABBINGGROUP_H

