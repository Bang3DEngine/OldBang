#include "Bang/IEventEmitter.h"

USING_NAMESPACE_BANG

void IEventEmitter::SetEmitEvents(bool emitEvents)
{
    m_emitEvents = emitEvents;
}

bool IEventEmitter::IsEmittingEvents() const
{
    return m_emitEvents;
}
