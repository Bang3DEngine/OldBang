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
        if (!IsStarted())
        {
            OnStart();
            m_started = true;
        }
    }
    virtual void _OnUpdate() { OnUpdate(); }
    virtual void _OnDestroy() { OnDestroy(); }

    #ifdef BANG_EDITOR
    virtual void _OnEditorStart()  { OnEditorStart(); }
    virtual void _OnEditorUpdate() { OnEditorUpdate(); }
    #endif


    virtual void _OnDrawGizmos(bool depthed, bool overlay)
    {
        #ifdef BANG_EDITOR
        OnDrawGizmos(depthed, overlay);
        #endif
    }

    virtual void OnStart()   {}
    virtual void OnUpdate()  {}
    virtual void OnDestroy() {}

    #ifdef BANG_EDITOR
    virtual void OnEditorStart()  {}
    virtual void OnEditorUpdate()  {}
    #endif

    virtual void OnDrawGizmos(bool depthed, bool overlay) {}
    bool IsStarted() const { return m_started; }

private:
    bool m_started = false;
};

#endif // SCENEEVENTLISTENER_H
