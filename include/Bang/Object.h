#ifndef OBJECT_H
#define OBJECT_H

#include "Bang/ObjectId.h"
#include "Bang/Serializable.h"
#include "Bang/IEventEmitter.h"
#include "Bang/IObjectListener.h"
#include "Bang/IDestroyListener.h"

NAMESPACE_BANG_BEGIN

class Object : public Serializable,
               public ICloneable,
               public IObjectListener,
               public EventEmitter<IDestroyListener>,
               public EventEmitter<IObjectListener>
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

    bool IsActive() const;

    virtual void PreStart();
    virtual void Start();

    virtual void OnPreStart();
    virtual void OnStart();
    virtual void OnEnabled() override;
    virtual void OnDisabled() override;
    virtual void OnDestroy();

    static void DestroyObject(Object *object);

private:
    ObjectId m_objectId;
    bool m_enabled = true;
    bool m_started = false;
    bool m_waitingToBeDestroyed = false;
};


NAMESPACE_BANG_END

#endif // OBJECT_H
