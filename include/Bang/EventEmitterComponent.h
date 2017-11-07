#ifndef EVENTEMITTERCOMPONENT_H
#define EVENTEMITTERCOMPONENT_H

#include "Bang/IEventEmitter.h"

NAMESPACE_BANG_BEGIN

template <class EventListenerClass>
class EventEmitterComponent : public IEventEmitter
{
public:
    template <class ReturnType, class... Args>
    void Propagate(ReturnType (EventListenerClass::*Function)(Args...),
                   Args&&... args) const;

    template <class ReturnType, class... Args>
    void PropagateInParent(bool recursive,
                           ReturnType (EventListenerClass::*Function)(Args...),
                           Args&&... args) const;

    template <class ReturnType, class... Args>
    void PropagateInChildrenOnly(
                   bool recursive,
                   ReturnType (EventListenerClass::*Function)(Args...),
                   Args&&... args) const;

protected:
    EventEmitterComponent() = default;
    virtual ~EventEmitterComponent() = default;

private:
    void UnRegisterListener(IEventListener*) {}
};

NAMESPACE_BANG_END

#include "EventEmitterComponent.tcc"

#endif // EVENTEMITTERCOMPONENT_H

