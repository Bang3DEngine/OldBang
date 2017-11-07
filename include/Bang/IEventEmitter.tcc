#include "Bang/IEventEmitter.h"

USING_NAMESPACE_BANG

template <class EListenerC>
void EventEmitter<EListenerC>::RegisterListener(EListenerC *listener)
{
    if (!m_listeners.Contains(listener))
    {
        m_listeners.PushBack(listener);
        listener->OnRegisteredTo(this);
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
