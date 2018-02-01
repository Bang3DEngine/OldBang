#include "Bang/Object.h"

#include "Bang/Debug.h"

USING_NAMESPACE_BANG

Object::~Object()
{
    ASSERT( IsWaitingToBeDestroyed() );
}

bool Object::IsActive() const
{
    return IsStarted() && IsEnabled() && !IsWaitingToBeDestroyed();
}

void Object::PreStart()
{
    if (!IsStarted())
    {
        OnPreStart();
    }
}

void Object::Start()
{
    if (!IsStarted())
    {
        OnStart();
        m_started = true;
        EventEmitter<IObjectListener>::
                PropagateToListeners(&IObjectListener::OnStarted);
    }
}

void Object::OnPreStart() {}
void Object::OnStart() {}
void Object::OnEnabled()
{
    EventEmitter<IObjectListener>::
            PropagateToListeners(&IObjectListener::OnEnabled);
}
void Object::OnDisabled()
{
    EventEmitter<IObjectListener>::
            PropagateToListeners(&IObjectListener::OnDisabled);
}
void Object::OnDestroy() {}

void Object::DestroyObject(Object *object)
{
    if (!object->IsWaitingToBeDestroyed())
    {
        object->m_waitingToBeDestroyed = true;

        object->OnDestroy();
        object->EventEmitter<IDestroyListener>::
                PropagateToListeners(&IDestroyListener::OnDestroyed, object);
    }
}

const ObjectId& Object::GetObjectId() const
{
    return m_objectId;
}

void Object::SetEnabled(bool enabled)
{
    if (enabled != IsEnabled())
    {
        m_enabled = enabled;
        if (IsEnabled()) { OnEnabled(); } else { OnDisabled(); }
    }
}

bool Object::IsEnabled() const { return m_enabled; }
bool Object::IsStarted() const { return m_started; }
bool Object::IsWaitingToBeDestroyed() const { return m_waitingToBeDestroyed; }

void Object::CloneInto(ICloneable *clone) const
{
    Object *obj = Cast<Object*>(clone);
    obj->SetEnabled( IsEnabled() );
}


