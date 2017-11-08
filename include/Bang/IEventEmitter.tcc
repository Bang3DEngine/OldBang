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
Propagate(ReturnType (EListenerC::*Function)(Args...), Args... args) const
{
    PropagateTo(m_listeners, Function, args...);
}

template <class EListenerC>
template <class T, class ReturnType, class... Args>
void EventEmitter<EListenerC>::
PropagateTo(const List<T*> &listenersList,
            ReturnType (EListenerC::*Function)(Args...),
            Args... args) const
{
    for (IEventListener *listener : listenersList)
    {
        PropagateTo(listener, Function, args...);
    }
}

template <class EListenerC>
template <class T, class ReturnType, class... Args>
void EventEmitter<EListenerC>::
PropagateTo(T* listener,
            ReturnType (EListenerC::*Function)(Args...),
            Args... args) const
{
    EListenerC *cListener = DCAST<EListenerC*>(listener);
    if (cListener) { (cListener->*Function)( std::forward<Args>(args)... ); }
}
