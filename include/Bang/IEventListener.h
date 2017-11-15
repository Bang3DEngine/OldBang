#ifndef IEVENTLISTENER_H
#define IEVENTLISTENER_H

#include "Bang/List.h"

NAMESPACE_BANG_BEGIN

FORWARD class IEventEmitter;

class IEventListener
{
public:
    void SetReceiveEvents(bool receiveEvents);
    bool IsReceivingEvents() const;

protected:
    IEventListener() = default;
    virtual ~IEventListener();

private:
    bool m_receivingEvents = true;
    List<IEventEmitter*> m_emitters;

    void OnRegisteredTo(IEventEmitter *emitter);
    void OnUnRegisteredFrom(IEventEmitter *emitter);

    template<class> friend class EventEmitter;
};

NAMESPACE_BANG_END

#endif // IEVENTLISTENER_H

