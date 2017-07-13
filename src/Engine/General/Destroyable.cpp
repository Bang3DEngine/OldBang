#include "Bang/Destroyable.h"

IDestroyListener::~IDestroyListener()
{
    for (Destroyable *destroyable : m_destroyablesRegisteredTo)
    {
        destroyable->UnRegisterDestroyListener(this);
    }
}


Destroyable::Destroyable()
{

}

Destroyable::~Destroyable()
{
    for (IDestroyListener *listener : m_destroyListeners)
    {
        listener->OnDestroyableDestroyed(this);
        listener->m_destroyablesRegisteredTo.Remove(this);
    }
}

void Destroyable::DemandDestroy()
{
    for (IDestroyListener *listener : m_destroyListeners)
    {
        listener->OnDestroyDemanded(this);
    }
}


void Destroyable::RegisterDestroyListener(IDestroyListener *listener)
{
    if (!m_destroyListeners.Contains(listener))
    {
        m_destroyListeners.PushBack(listener);
    }
    listener->m_destroyablesRegisteredTo.PushBack(this);
}

void Destroyable::UnRegisterDestroyListener(IDestroyListener *listener)
{
    m_destroyListeners.Remove(listener);
}
