#include "Bang/IEventEmitter.h"

#include "Bang/Component.h"
#include "Bang/GameObject.h"

USING_NAMESPACE_BANG

template <class EListenerC>
void EventEmitter<EListenerC>::RegisterListener(IEventListener *listener)
{
    EListenerC *listC = DCAST<EListenerC*>(listener);
    if (!m_listeners.Contains(listC))
    {
        m_listeners.PushBack(listC);
        listC->OnRegisteredTo(this);
    }
}

template <class EListenerC>
void EventEmitter<EListenerC>::UnRegisterListener(IEventListener *listener)
{
    m_listeners.Remove(listener);
    listener->OnUnRegisteredFrom(this);
}

template<class EventListenerClass>
EventEmitter<EventListenerClass>::~EventEmitter()
{
    while (!m_listeners.IsEmpty())
    {
        IEventListener *eListener = SCAST<IEventListener*>( m_listeners.Front() );
        UnRegisterListener(eListener);
    }
}

template <class EListenerC>
template <class ReturnType, class... Args>
void EventEmitter<EListenerC>::
Propagate(ReturnType (EListenerC::*Function)(Args...), Args&&... args)
{
    for (IEventListener *listener : m_listeners)
    {
        EListenerC *cListener = DCAST<EListenerC*>(listener);
        (cListener->*Function)( std::forward<Args>(args)... );
    }
}

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
