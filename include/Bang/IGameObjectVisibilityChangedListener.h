#ifndef IGAMEOBJECTVISIBILITYCHANGEDLISTENER_H
#define IGAMEOBJECTVISIBILITYCHANGEDLISTENER_H

#include "Bang/IEventListener.h"

NAMESPACE_BANG_BEGIN

FORWARD class Object;

class IGameObjectVisibilityChangedListener : public virtual IEventListener
{
    EVENTLISTENER(IGameObjectVisibilityChangedListener)

public:
    virtual void OnVisibilityChanged(GameObject *go) = 0;
};

NAMESPACE_BANG_END

#endif // IGAMEOBJECTVISIBILITYCHANGEDLISTENER_H
