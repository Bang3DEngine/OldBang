#ifndef IEVENTEMITTER_H
#define IEVENTEMITTER_H

#include "Bang/List.h"
#include "Bang/IEventListener.h"

NAMESPACE_BANG_BEGIN

class IEventEmitter
{
public:
    virtual void RegisterListener(IEventListener *listener) = 0;
    virtual void UnRegisterListener(IEventListener *listener) = 0;

private:
    IEventEmitter() = default;
    virtual ~IEventEmitter() = default;

    template<class> friend class EventEmitter;
    template<class> friend class EventEmitterComponent;
};



template <class EventListenerClass>
class EventEmitter : public IEventEmitter
{
public:
    void RegisterListener(IEventListener *listener) override;
    void UnRegisterListener(IEventListener *listener) override;

    template<class ReturnType, class... Args>
    void Propagate(ReturnType (EventListenerClass::*Function)(Args...),
                   Args&&... args);

protected:
    EventEmitter() = default;
    virtual ~EventEmitter();

private:
    List<IEventListener*> m_listeners;
};




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
    void RegisterListener(IEventListener*)   override {}
    void UnRegisterListener(IEventListener*) override {}
};

NAMESPACE_BANG_END

#include "Bang/IEventEmitter.tcc"

#endif // IEVENTEMITTER_H
