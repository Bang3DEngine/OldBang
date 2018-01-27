#include "Bang/Object.h"

USING_NAMESPACE_BANG

Object::~Object()
{
    ASSERT( IsWaitingToBeDestroyed() );
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
    }
}

void Object::OnPreStart() {}
void Object::OnStart() {}
void Object::OnEnabled()
{
    EventEmitter<IEnabledListener>::
            PropagateToListeners(&IEnabledListener::OnEnabled);
}
void Object::OnDisabled()
{
    EventEmitter<IEnabledListener>::
            PropagateToListeners(&IEnabledListener::OnDisabled);
}
void Object::OnDestroy() {}

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

    obj->m_started = false;
    obj->SetEnabled( IsEnabled() );
    obj->m_waitingToBeDestroyed = IsWaitingToBeDestroyed();
}

void Object::BeforeDestroyed()
{
    OnDestroy();
    EventEmitter<IDestroyListener>::
            PropagateToListeners(&IDestroyListener::OnDestroyed, this);
}


