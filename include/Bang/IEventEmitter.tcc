#include "Bang/IEventEmitter.h"

USING_NAMESPACE_BANG

template <class EListenerC>
void EventEmitter<EListenerC>::RegisterListener(EListenerC *listener)
{
    if (!m_iteratingListeners)
    {
        if (!m_listeners.Contains(listener))
        {
            m_listeners.PushBack(listener);
            listener->OnRegisteredTo(this);
        }
    }
    else
    { m_delayedListenersToRegister.PushBack(listener); }
}

template <class EListenerC>
void EventEmitter<EListenerC>::UnRegisterListener(IEventListener *listener)
{
    if (!m_iteratingListeners)
    {
        m_listeners.Remove(listener);
        listener->OnUnRegisteredFrom(this);
    }
    else
    { m_delayedListenersToUnRegister.PushBack(listener); }
}

template<class EventListenerClass>
const List<IEventListener*>& EventEmitter<EventListenerClass>::GetListeners() const
{
    return m_listeners;
}

template<class EventListenerClass>
EventEmitter<EventListenerClass>::~EventEmitter()
{
    while (!m_listeners.IsEmpty())
    {
        IEventListener *eListener = Cast<IEventListener*>( m_listeners.Front() );
        UnRegisterListener(eListener);
    }
}

