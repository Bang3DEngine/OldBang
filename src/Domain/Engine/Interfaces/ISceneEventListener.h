#ifndef ISCENEEVENTLISTENER_H
#define ISCENEEVENTLISTENER_H

#include "Bang.h"

#include "List.h"
#include <algorithm>
#include <functional>

#define PROPAGATE_EVENT(FUNCTION, ITERABLE) do {\
    for (auto it = (ITERABLE).Begin(); it != (ITERABLE).End(); ++it ) \
    {\
        if ((*it)->IsEnabled()) { \
            (*it)->FUNCTION();   \
        } \
    }\
} while (0)

class ISceneEventListener
{
protected:

    bool m_isStarted = false;

    ISceneEventListener () {}

    virtual void _OnStart ()
    {
        OnStart();
        m_isStarted = true;
    }

    virtual void _OnUpdate ()
    {
        OnUpdate();
    }

    virtual void _OnDestroy()
    {
        OnDestroy();
    }

    virtual void _OnDrawGizmos()
    {
        #ifdef BANG_EDITOR
        OnDrawGizmos();
        #endif
    }

    virtual void _OnDrawGizmosOverlay()
    {
        #ifdef BANG_EDITOR
        OnDrawGizmosOverlay();
        #endif
    }

    virtual void OnStart()   {}
    virtual void OnUpdate()  {}
    virtual void OnDestroy() {}
    virtual void OnDrawGizmos() {}
    virtual void OnDrawGizmosOverlay() {}

    bool IsStarted() const { return m_isStarted; }
};

#endif // SCENEEVENTLISTENER_H
