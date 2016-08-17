#ifndef ISCENEEVENTLISTENER_H
#define ISCENEEVENTLISTENER_H

#include "Bang.h"

#include <list>
#include <algorithm>
#include <functional>

#define PROPAGATE_EVENT(FUNCTION, ITERABLE) do {\
    for (auto it = (ITERABLE).begin(); it != (ITERABLE).end(); ++it ) \
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

    virtual void _OnPreRender ()
    {
        OnPreRender();
    }

    virtual void _OnRender ()
    {
        OnRender();
    }

    virtual void _OnDestroy()
    {
        OnDestroy();
    }

    #ifdef BANG_EDITOR
    virtual void _OnDrawGizmos()
    {
        OnDrawGizmos();
    }

    virtual void _OnDrawGizmosNoDepth()
    {
        OnDrawGizmosNoDepth();
    }
    #endif

    virtual void OnStart()   {}
    virtual void OnUpdate()  {}
    virtual void OnPreRender() {}
    virtual void OnRender() {}
    virtual void OnDestroy() {}
    #ifdef BANG_EDITOR
    virtual void OnDrawGizmos() {}
    virtual void OnDrawGizmosNoDepth() {}
    #endif

    bool IsStarted() const { return m_isStarted; }
};

#endif // SCENEEVENTLISTENER_H
