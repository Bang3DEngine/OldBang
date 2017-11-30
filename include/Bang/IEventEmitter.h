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
        m_iteratingListeners = true;
        for (const auto &x : GetListeners())
        {
            #ifdef DEBUG
            const int previousSize = GetListeners().Size();
            #endif

            PropagateToListener(x, func, args...);

            ASSERT(GetListeners().Size() == previousSize);
        }
        m_iteratingListeners = false;

        // Un/Register delayed listeners
        EventEmitter<EventListenerClass> *ncThis =
                        const_cast< EventEmitter<EventListenerClass>* >(this);
        for (IEventListener *listener : m_delayedListenersToRegister)
        {
            ncThis->RegisterListener( Cast<EventListenerClass*>(listener) );
        }
        for (IEventListener *listener : m_delayedListenersToUnRegister)
        {
            ncThis->UnRegisterListener( Cast<EventListenerClass*>(listener) );
        }
        m_delayedListenersToRegister.Clear();
        m_delayedListenersToUnRegister.Clear();
    }

    const List<IEventListener*>& GetListeners() const;

protected:
    EventEmitter() = default;
    virtual ~EventEmitter();

private:
    List<IEventListener*> m_listeners;

    mutable bool m_iteratingListeners = false;
    mutable List<IEventListener*> m_delayedListenersToRegister;
    mutable List<IEventListener*> m_delayedListenersToUnRegister;
};

NAMESPACE_BANG_END

#include "Bang/IEventEmitter.tcc"

#endif // IEVENTEMITTER_H
