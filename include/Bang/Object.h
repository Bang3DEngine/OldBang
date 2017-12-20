#ifndef OBJECT_H
#define OBJECT_H

#include "Bang/ObjectId.h"
#include "Bang/Serializable.h"
#include "Bang/IEventEmitter.h"
#include "Bang/IEnabledListener.h"
#include "Bang/IDestroyListener.h"

NAMESPACE_BANG_BEGIN

class Object : public virtual Serializable,
               public IEnabledListener,
               public EventEmitter<IDestroyListener>,
               public EventEmitter<IEnabledListener>
{
public:
    const ObjectId& GetObjectId() const;
    void SetEnabled(bool enabled);

    bool IsEnabled() const;
    bool IsStarted() const;
    bool IsWaitingToBeDestroyed() const;

    // ICloneable
    virtual void CloneInto(ICloneable *clone) const override;

protected:
    Object() = default;
    virtual ~Object();

    virtual void Start();
    virtual void OnStart();
    virtual void OnEnabled() override;
    virtual void OnDisabled() override;
    virtual void OnDestroy();

private:
    ObjectId m_objectId;
    bool m_enabled = true;
    bool m_started = false;
    bool m_waitingToBeDestroyed = false;

    void BeforeDestroyed();

    friend class ObjectManager;
};


NAMESPACE_BANG_END

#endif // OBJECT_H
