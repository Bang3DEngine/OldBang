#ifndef EVENTEMITTERCOMPONENT_TCC
#define EVENTEMITTERCOMPONENT_TCC

#include "Bang/Component.h"
#include "Bang/GameObject.h"

USING_NAMESPACE_BANG

template <class EListenerC>
template <class ReturnType, class... Args>
void EventEmitterComponent<EListenerC>::
Propagate(ReturnType (EListenerC::*Function)(Args...), Args&&... args) const
{
    const Component *thisComp = DCAST<const Component*>(this); ASSERT(thisComp);

    List<EListenerC*> componentListeners =
            thisComp->GetGameObject()->GetComponents<EListenerC>();
    for (EListenerC *listener : componentListeners)
    {
        (listener->*Function)( std::forward<Args>(args)... );
    }
}

template <class EListenerC>
template<class ReturnType, class... Args>
void EventEmitterComponent<EListenerC>::PropagateInParent(
                  bool recursive,
                  ReturnType (EListenerC::*Function)(Args...),
                  Args&&... args) const
{
    const Component *thisComp = DCAST<const Component*>(this); ASSERT(thisComp);

    List<EListenerC*> componentListeners =
        thisComp->GetGameObject()->GetComponentsInParent<EListenerC>(recursive);
    for (EListenerC *listener : componentListeners)
    {
        (listener->*Function)( std::forward<Args>(args)... );
    }
}

template <class EListenerC>
template<class ReturnType, class... Args>
void EventEmitterComponent<EListenerC>::
PropagateInChildrenOnly(bool recursive,
                        ReturnType (EListenerC::*Function)(Args...),
                        Args&&... args) const
{
    const Component *thisComp = DCAST<const Component*>(this); ASSERT(thisComp);

    List<EListenerC*> componentListeners =
        thisComp->GetGameObject()->GetComponentsInChildrenOnly<EListenerC>(recursive);
    for (EListenerC *listener : componentListeners)
    {
        (listener->*Function)( std::forward<Args>(args)... );
    }
}

#endif // EVENTEMITTERCOMPONENT_TCC
