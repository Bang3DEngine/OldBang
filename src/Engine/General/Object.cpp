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

Object::~Object()
{
    ASSERT( IsWaitingToBeDestroyed() );
}

void Object::BeforeDestroyed()
{
    OnDestroy();
    PROPAGATE(IDestroyListener, OnDestroyed, this);
}
