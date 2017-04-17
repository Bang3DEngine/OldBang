#ifndef ISCENEEVENTLISTENER_H
#define ISCENEEVENTLISTENER_H

#include "Bang/List.h"

#define PROPAGATE_EVENT(FUNCTION, ITERABLE) do {\
    for (auto it = (ITERABLE).Begin(); it != (ITERABLE).End(); ++it ) \
    {\
        if ((*it)->IsEnabled()) { \
            (*it)->FUNCTION;   \
        } \
    }\
} while (0)

class ISceneEventListener
{
protected:

    ISceneEventListener () {}

    virtual void _OnStart ()
    {
        OnStart();
    }

    virtual void _OnUpdate ()
    {
        OnUpdate();
    }

    #ifdef BANG_EDITOR
    virtual void _OnEditorUpdate ()
    {
        OnEditorUpdate();
    }
    #endif

    virtual void _OnDestroy()
    {
        OnDestroy();
    }

    virtual void _OnDrawGizmos(bool depthed, bool overlay)
    {
        #ifdef BANG_EDITOR
        OnDrawGizmos(depthed, overlay);
        #endif
    }

    virtual void OnStart()   {}
    virtual void OnUpdate()  {}
    #ifdef BANG_EDITOR
    virtual void OnEditorUpdate()  {}
    #endif
    virtual void OnDestroy() {}
    virtual void OnDrawGizmos(bool depthed, bool overlay) {}
};

#endif // SCENEEVENTLISTENER_H
