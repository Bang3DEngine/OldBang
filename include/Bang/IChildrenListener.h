#ifndef ICHILDRENLISTENER_H
#define ICHILDRENLISTENER_H

#include "Bang/IEventListener.h"

NAMESPACE_BANG_BEGIN

class IChildrenListener : public virtual IEventListener
{
public:
    virtual void OnChildAdded(GameObject *addedChild, GameObject *parent)
    {
    };

    virtual void OnChildRemoved(GameObject *removedChild, GameObject *parent)
    {
    };
};

NAMESPACE_BANG_END

#endif // ICHILDRENLISTENER_H
