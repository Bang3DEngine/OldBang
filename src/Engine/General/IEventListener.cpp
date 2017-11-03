#include "Bang/IEventListener.h"

#include "Bang/IEventEmitter.h"

USING_NAMESPACE_BANG

IEventListener::~IEventListener()
{
    while (!m_emitters.IsEmpty())
    {
        m_emitters.Front()->UnRegisterListener(this);
    }
}

void IEventListener::OnRegisteredTo(IEventEmitter *emitter)
{
    m_emitters.PushBack(emitter);
}

void IEventListener::OnUnRegisteredFrom(IEventEmitter *emitter)
{
    m_emitters.Remove(emitter);
}
