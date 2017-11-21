#include "Bang/Object.h"

USING_NAMESPACE_BANG

void Object::Start()
{
    if (!IsStarted())
    {
        OnStart();
        m_started = true;
    }
}
void Object::OnStart() {}
void Object::OnEnabled()
{
    PROPAGATE(IEnabledListener, OnEnabled);
}
void Object::OnDisabled()
{
    PROPAGATE(IEnabledListener, OnDisabled);
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
    Object *obj = DCAST<Object*>(clone);

    obj->m_started = false;
    obj->SetEnabled( IsEnabled() );
    obj->m_waitingToBeDestroyed = IsWaitingToBeDestroyed();
}

Object::~Object()
{
    ASSERT( IsWaitingToBeDestroyed() );
}

void Object::BeforeDestroyed()
{
    OnDestroy();
    PROPAGATE(IDestroyListener, OnDestroyed, this);
}

// ObjectId

ObjectId::ObjectIdType ObjectId::s_nextObjectId = 0;

ObjectId::ObjectId()
{
    m_id = ObjectId::s_nextObjectId;
    ++ObjectId::s_nextObjectId;
}

bool operator!=(const ObjectId &lhs, const ObjectId &rhs)
{ return !(lhs == rhs); }
