#ifndef IEVENTEMITTER_H
#define IEVENTEMITTER_H

#include <tuple>

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

    template<class TListener, class TFunction, class... Args>
    static void PropagateToListener(const TListener &listener,
                                    const TFunction &func,
                                    const Args&... args)
    {
        if (listener && listener->IsReceivingEvents())
        {
            (Cast<EventListenerClass*>(listener)->*func)(args...);
        }
    }

    template<class TFunction, class... Args>
    void PropagateToListeners(const TFunction &func, const Args&... args) const
    {
        for (const auto &x : GetListeners())
        {
            PropagateToListener(x, func, args...);
        }
    }

    const List<IEventListener*>& GetListeners() const;

protected:
    EventEmitter() = default;
    virtual ~EventEmitter();

private:
    List<IEventListener*> m_listeners;
};

NAMESPACE_BANG_END

#include "Bang/IEventEmitter.tcc"

#endif // IEVENTEMITTER_H
