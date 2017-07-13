#ifndef DESTROYABLE_H
#define DESTROYABLE_H

#include "Bang/List.h"

class Destroyable;
class IDestroyListener
{
protected:
    virtual ~IDestroyListener();
    virtual void OnDestroyableDestroyed(Destroyable *destroyedObject)   {}
    virtual void OnDestroyDemanded(Destroyable *objectDemandingDestroy) {}

private:
    List<Destroyable*> m_destroyablesRegisteredTo;

    friend class Destroyable;
};

class Destroyable
{
public:
    void RegisterDestroyListener(IDestroyListener *listener);

protected:
    Destroyable();
    virtual ~Destroyable();

    void DemandDestroy();

private:
    List<IDestroyListener*> m_destroyListeners;
    void UnRegisterDestroyListener(IDestroyListener *listener);

    friend class IDestroyListener;
};

#endif // DESTROYABLE_H
