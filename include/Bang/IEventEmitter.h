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

    const List<IEventListener*> GetListeners() const;

protected:
    EventEmitter() = default;
    virtual ~EventEmitter();

private:
    List<IEventListener*> m_listeners;
};

// PROPAGATE Macros
#define _PROPAGATE(EventListenerClass, FunctionCall, List) \
{ \
    static_assert(T_SUBCLASS(EventListenerClass, IEventListener), \
                  ""#EventListenerClass" must be subclass of IEventListener.");\
    const auto l = List;\
    for (IEventListener *listener : l) \
    { \
        EventListenerClass *cListener = DCAST<EventListenerClass*>(listener); \
        if (cListener && cListener->IsReceivingEvents()) \
        { cListener->FunctionCall; } \
    } \
}

#define PROPAGATE(ListenerClass, ListenerFunction, ...) \
    { _PROPAGATE(ListenerClass, ListenerFunction(__VA_ARGS__), \
                 EventEmitter<ListenerClass>::GetListeners()); }
#define PROPAGATE_1(ListenerClass, ListenerFunction, ExtraListenersList, ...) \
{ \
    PROPAGATE(ListenerClass, ListenerFunction, __VA_ARGS__); \
    _PROPAGATE(ListenerClass, ListenerFunction(__VA_ARGS__), ExtraListenersList); \
}
#define PROPAGATE_2(ListenerClass, ListenerFunction, \
                    ExtraListenersList1, ExtraListenersList2, ...) \
{ \
    PROPAGATE_1(ListenerClass, ListenerFunction, ExtraListenersList1, __VA_ARGS__); \
    _PROPAGATE(ListenerClass, ListenerFunction(__VA_ARGS__), ExtraListenersList2); \
}
#define PROPAGATE_3(ListenerClass, ListenerFunction, \
                    ExtraListenersList1, ExtraListenersList2, ExtraListenersList3, ...) \
{ \
    PROPAGATE_2(ListenerClass, ListenerFunction, ExtraListenersList1, ExtraListenersList2, __VA_ARGS__); \
    _PROPAGATE(ListenerClass, ListenerFunction(__VA_ARGS__), ExtraListenersList3); \
}
#define PROPAGATE_4(ListenerClass, ListenerFunction, \
                    ExtraListenersList1, ExtraListenersList2, ExtraListenersList3, ExtraListenersList4, ...) \
{ \
    PROPAGATE_3(ListenerClass, ListenerFunction, ExtraListenersList1, ExtraListenersList2, ExtraListenersList3, __VA_ARGS__); \
    _PROPAGATE(ListenerClass, ListenerFunction(__VA_ARGS__), ExtraListenersList4); \
}


NAMESPACE_BANG_END

#include "Bang/IEventEmitter.tcc"

#endif // IEVENTEMITTER_H
