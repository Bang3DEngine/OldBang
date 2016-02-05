#ifndef STAGEEVENTLISTENER_H
#define STAGEEVENTLISTENER_H

#include <list>
#include <algorithm>
#include <functional>

#define PROPAGATE_EVENT(FUNCTION, ITERABLE) \
    for(auto it = (ITERABLE).begin(); it != (ITERABLE).end(); ++it ) \
    {\
        (*it)->FUNCTION(); \
    }

class StageEventListener
{
protected:

    StageEventListener() {}

    virtual void _OnStart()   { OnStart();   }
    virtual void _OnUpdate()  { OnUpdate();  }
    virtual void _OnDrawing() { OnDrawing(); }
    virtual void _OnDestroy() { OnDestroy(); }

    virtual void OnStart()   {}
    virtual void OnUpdate()  {}
    virtual void OnDrawing() {}
    virtual void OnDestroy() {}
};

#endif // STAGEEVENTLISTENER_H
