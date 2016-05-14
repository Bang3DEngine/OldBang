#ifndef ISCENEEVENTLISTENER_H
#define ISCENEEVENTLISTENER_H

#include "Bang.h"

#include <list>
#include <algorithm>
#include <functional>

#define PROPAGATE_EVENT(FUNCTION, ITERABLE) do {\
    for(auto it = (ITERABLE).begin(); it != (ITERABLE).end(); ++it ) \
    {\
        if((*it)->IsEnabled()) { \
            (*it)->FUNCTION();   \
        } \
    }\
} while(0)

#define PROPAGATE_EVENT_WITH_RENDER_LAYER(RLAYER, FUNCTION, ITERABLE) do {\
    for(auto it = (ITERABLE).begin(); it != (ITERABLE).end(); ++it ) \
    {\
        if((*it)->IsEnabled()) { \
            (*it)->FUNCTION(RLAYER);   \
        } \
    }\
} while(0)


class ISceneEventListener
{
protected:

    ISceneEventListener() {}

    virtual void _OnStart()   { OnStart();   }
    virtual void _OnUpdate()  { OnUpdate();  }
    virtual void _OnPreRender (unsigned char renderLayer = 255)
    {
        //Components won't ever be called with a renderLayer,
        // so the OnPreRender method will be called automatically
        if(renderLayer == 255)
            OnPreRender();
    }
    virtual void _OnRender    (unsigned char renderLayer = 255)
    {   //Same happens here
        if(renderLayer == 255)
            OnRender();
    }
    virtual void _OnDestroy() { OnDestroy(); }

    virtual void OnStart()   {}
    virtual void OnUpdate()  {}
    virtual void OnPreRender() {}
    virtual void OnRender() {}
    virtual void OnDestroy() {}
};

#endif // SCENEEVENTLISTENER_H
