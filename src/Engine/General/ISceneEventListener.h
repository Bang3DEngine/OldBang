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

    virtual void OnStart()   {}
    virtual void OnUpdate()  {}
    virtual void OnDestroy() {}

    bool IsStarted() const { return m_started; }

private:
    bool m_started = false;
};

#endif // SCENEEVENTLISTENER_H
