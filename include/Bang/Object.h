#ifndef OBJECT_H
#define OBJECT_H

#include "Bang/IEventEmitter.h"
#include "Bang/IEnabledListener.h"
#include "Bang/IDestroyListener.h"

NAMESPACE_BANG_BEGIN

class Object : public EventEmitter<IDestroyListener>,
               public EventEmitter<IEnabledListener>
{
public:
    void SetEnabled(bool enabled);

    bool IsEnabled() const;
    bool IsStarted() const;
    bool IsWaitingToBeDestroyed() const;

protected:
    Object() = default;
    virtual ~Object();

    virtual void Start();
    virtual void OnStart();
    virtual void OnEnabled();
    virtual void OnDisabled();
    virtual void OnDestroy();

private:
    bool m_enabled = true;
    bool m_started = false;
    bool m_waitingToBeDestroyed = false;

    void BeforeDestroyed();

    friend class ObjectManager;
};

NAMESPACE_BANG_END

#endif // OBJECT_H
