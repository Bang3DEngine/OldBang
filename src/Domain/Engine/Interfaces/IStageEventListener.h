#ifndef ISTAGEEVENTLISTENER_H
#define ISTAGEEVENTLISTENER_H

#include <list>
#include <algorithm>
#include <functional>

#define PROPAGATE_EVENT(FUNCTION, ITERABLE) do {\
    for(auto it = (ITERABLE).begin(); it != (ITERABLE).end(); ++it ) \
    {\
        (*it)->FUNCTION(); \
    }\
} while(0)


class IStageEventListener
{
protected:

    IStageEventListener() {}

    virtual void _OnStart()   { OnStart();   }
    virtual void _OnUpdate()  { OnUpdate();  }
    virtual void _OnRender() { OnRender(); }
    virtual void _OnDestroy() { OnDestroy(); }

    virtual void OnStart()   {}
    virtual void OnUpdate()  {}
    virtual void OnRender() {}
    virtual void OnDestroy() {}
};

#endif // STAGEEVENTLISTENER_H
