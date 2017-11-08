#ifndef IEVENTEMITTER_H
#define IEVENTEMITTER_H

#include "Bang/List.h"
#include "Bang/Array.h"
#include "Bang/IEventListener.h"

NAMESPACE_BANG_BEGIN

class IEventEmitter
{
public:
    virtual void UnRegisterListener(IEventListener *listener) = 0;

private:
    IEventEmitter() = default;
    virtual ~IEventEmitter() = default;

    template<class> friend class EventEmitter;
};

template <class EventListenerClass>
class EventEmitter : public IEventEmitter
{
public:
    void RegisterListener(EventListenerClass *listener);
    void UnRegisterListener(IEventListener *listener) override;

    template<class ReturnType, class... Args>
    void Propagate(ReturnType (EventListenerClass::*Function)(Args...),
                   Args... args) const;

    // Extra propagate to one list
    template<class T, class ReturnType, class... Args>
    void PropagateTo(const List<T*> &listenersList,
                     ReturnType (EventListenerClass::*Function)(Args...),
                     Args... args) const;

    template<class T, class ReturnType, class... Args>
    void PropagateTo(T* listener,
                     ReturnType (EventListenerClass::*Function)(Args...),
                     Args... args) const;

protected:
    EventEmitter() = default;
    virtual ~EventEmitter();

private:
    List<IEventListener*> m_listeners;
};

// PROPAGATE Macros

#define PROPAGATE(ListenerClass, ListenerFunction, ...) \
    { EventEmitter<ListenerClass>::Propagate(&ListenerClass::ListenerFunction, __VA_ARGS__); }
#define PROPAGATE_0(ListenerClass, ListenerFunction) \
    { EventEmitter<ListenerClass>::Propagate(&ListenerClass::ListenerFunction); }

#define PROPAGATE_1_0(ListenerClass, ListenerFunction, \
                      ExtraListenersList) \
{ \
    PROPAGATE_0(ListenerClass, ListenerFunction); \
    EventEmitter<ListenerClass>::PropagateTo(ExtraListenersList, &ListenerClass::ListenerFunction); \
}
#define PROPAGATE_1(ListenerClass, ListenerFunction, \
                    ExtraListenersList, ...) \
{ \
    PROPAGATE(ListenerClass, ListenerFunction, __VA_ARGS__); \
    EventEmitter<ListenerClass>::PropagateTo(ExtraListenersList, &ListenerClass::ListenerFunction, __VA_ARGS__); \
}

#define PROPAGATE_2_0(ListenerClass, ListenerFunction, \
                      ExtraListenersList1, ExtraListenersList2) \
{ \
    PROPAGATE_1_0(ListenerClass, ListenerFunction, ExtraListenersList1); \
    EventEmitter<ListenerClass>::PropagateTo(ExtraListenersList2, &ListenerClass::ListenerFunction); \
}
#define PROPAGATE_2(ListenerClass, ListenerFunction, \
                    ExtraListenersList1, ExtraListenersList2, \
                    ...) \
{ \
    PROPAGATE_1(ListenerClass, ListenerFunction, ExtraListenersList1, __VA_ARGS__); \
    EventEmitter<ListenerClass>::PropagateTo(ExtraListenersList2, &ListenerClass::ListenerFunction, __VA_ARGS__); \
}

#define PROPAGATE_3_0(ListenerClass, ListenerFunction, \
                      ExtraListenersList1, ExtraListenersList2, ExtraListenersList3) \
{ \
    PROPAGATE_2_0(ListenerClass, ListenerFunction, ExtraListenersList1, ExtraListenersList2); \
    EventEmitter<ListenerClass>::PropagateTo(ExtraListenersList3, &ListenerClass::ListenerFunction); \
}
#define PROPAGATE_3(ListenerClass, ListenerFunction, \
                    ExtraListenersList1, ExtraListenersList2, ExtraListenersList3, \
                    ...) \
{ \
    PROPAGATE_2(ListenerClass, ListenerFunction, ExtraListenersList1, ExtraListenersList2, __VA_ARGS__); \
    EventEmitter<ListenerClass>::PropagateTo(ExtraListenersList3, &ListenerClass::ListenerFunction, __VA_ARGS__); \
}


NAMESPACE_BANG_END

#include "Bang/IEventEmitter.tcc"

#endif // IEVENTEMITTER_H
